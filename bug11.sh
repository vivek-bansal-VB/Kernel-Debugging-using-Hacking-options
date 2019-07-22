  #!/bin/sh                                                                       
                                                                                  
  echo "######################"                                                   
  echo "######################"                                                   
  echo "Running code to trigger BUG11 - Priority Linked list manipulation"               
  echo "######################"                                                   
  echo "######################"                                                   
  echo "running user space program to check bug11"                                 
  ../xhw3 11                            
  dmesg | tail  -100                                         
