#! /bin/bash
#��ӡ10���ڵļӷ���
i=1
while [ $i -le 9 ]
    do
       row=1
       colume=$i
       while [ $row -le $i ]
          do
            if((i==9))
             then
               echo -n "$row+$colume=$((row+colume)) "
            else
               echo -n "$row+$colume=$((row+colume)) "
            fi
            row=$(expr $row + 1)
            colume=$(expr $colume - 1)
          done
       echo
       ((i=$i+1))
done