#!/usr/bin/env python3
import os
import sys
import time
import signal
import subprocess
import select
import fcntl

class MinishellTester:
    def __init__(self, minishell_path="./minishell"):
        self.minishell_path = minishell_path
        self.process = None
        self.timeout = 1  # Timeout en segundos para esperar respuesta

    def start_minishell(self):
        """Inicia el proceso de minishell"""
        self.process = subprocess.Popen(
            self.minishell_path,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,
            preexec_fn=os.setsid  # Necesario para enviar señales al grupo
        )
        
        # Establecer modo no bloqueante para stdout
        flags = fcntl.fcntl(self.process.stdout, fcntl.F_GETFL)
        fcntl.fcntl(self.process.stdout, fcntl.F_SETFL, flags | os.O_NONBLOCK)

    def send_command(self, command):
        """Envía un comando a minishell"""
        if not self.process:
            self.start_minishell()
        
        print(f"\n> Enviando comando: {command}")
        self.process.stdin.write(command + "\n")
        self.process.stdin.flush()
        
        # Esperar y recoger la salida
        return self.get_output()

    def send_eof(self):
        """Simula Ctrl+D (EOF)"""
        print("\n> Enviando EOF (Ctrl+D)")
        self.process.stdin.close()
        return self.get_output()

    def send_sigint(self):
        """Simula Ctrl+C (SIGINT)"""
        if not self.process:
            return "Minishell no está en ejecución"
        
        print("\n> Enviando SIGINT (Ctrl+C)")
        os.killpg(os.getpgid(self.process.pid), signal.SIGINT)
        return self.get_output()

    def get_output(self):
        """Recoge la salida de minishell con timeout"""
        output = ""
        start_time = time.time()
        
        while True:
            # Comprobar si el proceso ha terminado
            if self.process.poll() is not None:
                break
                
            # Ver si hay datos disponibles para leer
            ready_to_read, _, _ = select.select([self.process.stdout, self.process.stderr], [], [], 0.1)
            
            for stream in ready_to_read:
                try:
                    line = stream.readline()
                    if line:
                        output += line
                except IOError:
                    pass
            
            # Comprobar timeout
            if time.time() - start_time > self.timeout:
                break
        
        return output.strip()

    def run_test_file(self, test_file):
        """Ejecuta pruebas desde un archivo"""
        with open(test_file, 'r') as f:
            tests = f.read().splitlines()
        
        self.start_minishell()
        results = []
        
        for test in tests:
            if test.startswith("#SIGINT"):
                time.sleep(0.5)  # Dar tiempo entre comandos
                results.append(self.send_sigint())
            elif test.startswith("#EOF"):
                time.sleep(0.5)
                results.append(self.send_eof())
            else:
                time.sleep(0.5)
                results.append(self.send_command(test))
        
        self.close()
        return results

    def run_heredoc_sigint_test(self):
        """Prueba específica para heredoc con SIGINT"""
        self.start_minishell()
        
        # Iniciar un heredoc
        print("\n> Probando heredoc con SIGINT")
        self.process.stdin.write("cat << EOF\n")
        self.process.stdin.flush()
        time.sleep(0.5)
        
        # Enviar una línea
        self.process.stdin.write("línea de prueba\n")
        self.process.stdin.flush()
        time.sleep(0.5)
        
        # Enviar SIGINT en lugar del delimitador
        result = self.send_sigint()
        self.close()
        return result

    def close(self):
        """Cierra el proceso de minishell"""
        if self.process and self.process.poll() is None:
            self.process.terminate()
            try:
                self.process.wait(timeout=1)
            except subprocess.TimeoutExpired:
                self.process.kill()

if __name__ == "__main__":
    tester = MinishellTester()
    
    # Ejemplo de uso:
    # 1. Ejecutar comandos normales
    tester.start_minishell()
    print(tester.send_command("ls -la"))
    print(tester.send_command("echo hola"))
    
    # 2. Probar SIGINT
    print(tester.send_command("sleep 5"))
    time.sleep(1)  # Esperar un poco antes de interrumpir
    print(tester.send_sigint())
    
    # 3. Probar heredoc con SIGINT
    tester.close()
    print(tester.run_heredoc_sigint_test())
    
    # 4. Probar EOF
    tester.start_minishell()
    print(tester.send_command("echo prueba de EOF"))
    print(tester.send_eof())
    
    # Ejemplo de uso con archivo de pruebas:
    # tester.run_test_file("test_commands.txt")
