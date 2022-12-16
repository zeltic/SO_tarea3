# SO_tarea3
Tarea 3 de Sistemas Operativos

Comandos:
{
  mkdir:
  {
    Desc: Crea directorios
    Uso: mkdir Directorio (mkdir micarpeta ; mkdir Desktop/micarpeta ; mkdir ../Desktop/micarpeta ; mkdir ~/Desktop/micarpeta)
  }
  
  touch:
  {
    Desc: Crea archivos
    Uso: touch Archivo (touch miarchivo ; touch Desktop/miarchivo ; touch ../Desktop/miarchivo ; touch ~/Desktop/miarchivo)
  }
  
  cd:
  {
    Desc: Cambia el directorio actual
    Uso: cd Direccion (cd micarpeta; cd Desktop/micarpeta ; cd ../.. ; cd ~/Desktop/micarpeta)
  }
  
  ls:
  {
    Desc: Muestra el contenido del directorio
    Uso: ls [Opcion] (ls -s)
    Opciones: 
    {
      -i  imprime el indice
      -s  imprime el tamaño
      -R  imprime todo el contenido de manera recursiva
    }
  }
  
  mv:
  {
    Desc: Copia el archivo o directorio a una dirección nueva; eliminando el antiguo. Se puede usar para renombrar.
    Uso: mv Fuente Destino (mv micarpeta ../micarpeta ; mv Desktop/nombre Desktop/nuevonombre)
  }
  
  cp:
  {
    Desc: Copia el archivo o directorio (la id de los archivos nuevos cambiara).
    Uso: cp Fuente Destino (cp micarpeta Desktop/micarpeta)
  }
  
  rm:
  {
    Desc: Elimina un archivo o directorio. El contenido del directorio se eliminara de manera recursiva.
    Uso: rm Direccion (rm /usr)
  }
  
  exit:
  {
    Desc: Termina el programa
    Uso: exit
  }
}
