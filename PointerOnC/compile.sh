#!/bin/sh

gcc ./ch1/rearrang.c -g -Wall  -o rearrang

gcc ./ch7/parity.c -g -Wall  -o parity
gcc ./ch7/swap1.c -g -Wall -o swap1
gcc ./ch7/swap2.c -g -Wall -o swap2
gcc ./ch7/btoa.c -g -Wall -o btoa
gcc ./ch7/fact_rec.c -g -Wall -o fact_rec
gcc ./ch7/fact_itr.c -g -Wall -o fact_itr
gcc ./ch7/fib_rec.c -g -Wall -o fib_rec
gcc ./ch7/fib_iter.c -g -Wall -o fib_iter
gcc ./ch7/average2.c -g -Wall -o average2

gcc ./ch8/strcpy.c -g -Wall -o strcpy
gcc ./ch8/keyword.c -g -Wall -o keyword

gcc ./ch9/mstrrstr.c -g -Wall -o mstrrstr
gcc ./ch9/token.c -g -Wall -o token

gcc ./ch11/alloc.c ./ch11/a_client.c -g -Wall -o a_client
gcc ./ch11/sort.c -g -Wall -o sort
gcc ./ch11/strdup.c -g -Wall -o strdup

gcc ./ch12/insert1.c -g -Wall -o insert1
gcc ./ch12/insert2.c -g -Wall -o insert2
gcc ./ch12/insert3.c -g -Wall -o insert3

gcc ./ch13/search.c -g -Wall -o search
gcc ./ch13/echo.c -g -Wall -o echo
gcc ./ch13/cmd_line.c -g -Wall -o cmd_line
gcc ./ch13/mystery.c -g -Wall -o mystery
gcc ./ch13/btoa.c -g -Wall -o btoa

gcc ./ch16/shuffle.c -g -Wall -o shuffle

gcc ./ch17/a_stack.c -g -Wall -o a_stack
gcc ./ch17/d_stack.c -g -Wall -o d_stack
gcc ./ch17/l_stack.c -g -Wall -o l_stack
gcc ./ch17/a_queue.c -g -Wall -o a_queue
gcc ./ch17/d_queue.c -g -Wall -o d_queue
gcc ./ch17/l_queue.c -g -Wall -o l_queue
gcc ./ch17/a_tree.c -g -Wall -o a_tree

