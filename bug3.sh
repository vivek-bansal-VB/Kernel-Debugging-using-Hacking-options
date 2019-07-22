  #!/bin/sh                                                                       
                                                                                  
  echo "######################"                                                   
  echo "######################"                                                   
  echo "Running code to trigger BUG3 - Sleep inside atomic section"               
  echo "######################"                                                   
  echo "######################"                                                   
  echo "running user space program to check bug3"                                 
  ../xhw3 3   
  dmesg | tail -40
