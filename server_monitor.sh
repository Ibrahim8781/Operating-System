#!/bin/bash

LOG_FILE="./server.log"

# disk_usage=$(df / | grep /)
disk_usage=$(df / | grep / | awk '{print $5}' | sed 's/%//')

  echo "$(date): Disk usage is $disk_usage%" >> $LOG_FILE
if [ "$disk_usage" -gt 50 ]; then
  echo "Alert: Disk usage is above 50%!" >> $LOG_FILE
else
    echo "Disk usage is normal." >> $LOG_FILE
fi

cpu_idle=$(top  -bn1 | grep "Cpu(s)" | awk '{print $8}' )
cpu_usage=$(echo "100 - $cpu_idle" | bc)  

  echo "$(date): CPU usage is $cpu_usage%" >> $LOG_FILE
if (( $(echo "$cpu_usage > 50" | bc -l) )); then
  echo " Alert: CPU usage is above 50%!" >> $LOG_FILE
else
    echo "CPU usage is normal." >> $LOG_FILE
fi


total_memory=$(free -m | grep Mem | awk '{print $2}')
available_memory=$(free -m | grep Mem | awk '{print $7}')
memory_usage=$(echo "($available_memory / $total_memory) * 100" | bc -l)

  echo "$(date): Total memory is: $total_memory" >> $LOG_FILE
if (( $(echo "$memory_usage < 30" | bc -l) )); then
  echo "Alert: Available memory is below 30%!" >> $LOG_FILE
else
    echo "Available memory is normal." >> $LOG_FILE
    echo "$(date): Available memory is: $available_memory" >> $LOG_FILE
    echo "$(date): Current available memory is: $memory_usage%" >> $LOG_FILE
fi
