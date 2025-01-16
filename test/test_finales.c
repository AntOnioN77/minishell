 "echo hello > outfile"                    // Redirección de salida básica
"cat < infile > outfile"                  // Combinación de redirecciones in/out
"echo 'hello world' >> appendfile"        // Redirección append
"cat << EOF"                             // Heredoc básico
"cat << DELIM >> outfile"                // Heredoc con redirección
Comillas y variables de entorno:
 "echo \"$HOME\""                         // Variable dentro de comillas dobles
"echo '$HOME'"                           // Variable dentro de comillas simples (no debe expandirse)
"echo \"hello'world'\""                  // Comillas simples dentro de dobles
"echo 'hello\"world\"'"                  // Comillas dobles dentro de simples
"echo $USER$HOME$PATH"                   // Múltiples variables juntas
"echo \"$USER\"'$HOME'\"$PATH\""        // Combinación de comillas y variables
Pipes complejos:
 "cat file.txt | grep a | sort -r | uniq" // Pipe múltiple con varios comandos
"ls -l | grep .txt | sort | head -n 5"   // Pipe con limitación de output
"echo hello | cat -e | cat -e | cat -e"  // Pipe con mismo comando repetido
Casos especiales:
 "   ls    -l     -a    "                // Múltiples espacios entre argumentos
"ls -la|grep a|wc -l"                   // Sin espacios alrededor de pipes
"echo hola>file"                        // Sin espacio antes de redirección
"<<EOF cat"                             // Heredoc antes del comando
"cat infile1 < infile2"                 // Redirección con múltiples argumentos
Casos de error que debes manejar:
 "echo \"hello"                          // Comillas dobles sin cerrar
"echo 'world"                           // Comillas simples sin cerrar
"ls > > outfile"                        // Redirección mal formada
"ls | | grep a"                         // Pipe mal formado
"cat < nonexistentfile"                 // Archivo que no existe
"<<<EOF"                                // Heredoc mal formado
Combinaciones complejas:
 "cat < input.txt | grep a > output.txt" // Redirección entrada/salida con pipe
"echo \"$USER\" > file1 | cat < file1"  // Variable, redirección y pipe
"ls -la > file1 >> file2 | wc -l"       // Múltiples redirecciones con pipe
"cat << EOF | grep a >> outfile"        // Heredoc con pipe y append
"echo $USER > \"$HOME/test file.txt\""  // Variable en ruta con espacios
Casos límite:
 ""                                      // String vacío
" "                                     // Solo espacio
"|||"                                   // Múltiples pipes sin comandos
">>>"                                   // Múltiples redirecciones sin comandos
"$"                                     // Solo signo de dólar
">"                                     // Solo signo de redirección