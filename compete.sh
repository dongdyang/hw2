#!/bin/bash
star="*"
p1_score=0
p2_score=0
p1_time=0
p2_time=0

# the shell is run on LINUX/MAC.
# pay attention that the score you earn at each step, you need to add it the second-final line
# and the time you run, you need to add it to the final line.

#input example:
: '
10
300
8
2512154223
1221241443
5553242103
5402340245
1041243302
4451310451
4535503102
2052125402
1155241015
2250115141
'

# output example:
: '
D10
**********
**********
**********
**********
**********
**********
**********
**********
**********
**********
1
0.000101
'

# here is 1/2 part you need to modify. C++/java
g++ p1.cpp -o player1 -std=c++11
g++ p2.cpp -o player2 -std=c++11
#javac p1.java
#javac p2.java


epoch=1
which=0     #which player run first
while true
do
    count=0

    # here is 2/2 part you need to modify C++/java
    if [ $which == 0 ]; then
        ./player1
        #java p1
        let which=1
    else
        ./player2
        #java p2
        let which=0
    fi

    #-----------------------------------you do not  need to care below
    size=`sed -n '1p' input.txt`
    #echo $size
    lineNum=0
    while read LINE
    do
        if [[ 1 -le $lineNum ]] && [[ $lineNum -le $size ]];
        then
            flag=0
            for i in `seq ${#LINE}`
            do
                if [ "${LINE:$i-1:1}" != "$star" ];
                then
                    let flag=1            
                fi
            done
            #echo $flag, $lineNum
            if [ $flag -eq 0 ];
            then
                let count++ 
            fi
        fi
        
        if [ $lineNum == $[$size+1] ]; 
        then
            #echo "-----","$epoch", "$which"
            if [ $which == 1 ];then
                let p1_score+=$LINE
            else
                let p2_score+=$LINE
            fi
        fi
        
        
        if [ $lineNum == $[$size+2] ]; 
        then
            if [ $which == 1 ];then
                #p1_time=`expr $p1_time + $LINE`
                #p1_time=$(awk 'BEGIN{print $p1_time + $LINE}')
                p1_time=$(echo "$p1_time+$LINE" | bc)
            else
                p2_time=$(echo "$p2_time+$LINE" | bc)
                #p2_time=$(awk 'BEGIN{print $p2_time + $LINE}')
                #p2_time=`expr $p2_time + $LINE`
            fi
        fi

        let lineNum++

    done< output.txt

    if [ $(echo "$p1_time > 300" | bc) -eq 1 ]; then
        echo "player 1 time-out fail"
        exit
    fi
    if [ $(echo "$p2_time > 300" | bc) -eq 1 ]; then
    #if [ "$p2_time" -gt "300" ]; then
        echo "player 2 time-out fail"
        exit
    fi

    echo "Epoch $epoch", "P1",$p1_score, "P2", $p2_score

    #echo $count, $size
    if [ "$count" == "$size" ]; then
        echo "Game Over"
        echo "p1 ", $p1_score
        echo "p2 ", $p2_score
        if [ $p1_score -gt $p2_score ]; then
            echo "player 1 win"
        elif [ $p1_score -lt $p2_score ]; then
            echo "player 2 win"
        else
            echo "player 1 and 2 are two-win"
        fi
        exit 
    fi

        
    let epoch+=1
    lastInputLine=$[$size+1]
    #echo $lastInputLine
    sed -ig '4, $d' input.txt
    sed -n '2, '"$lastInputLine"'p' output.txt  >> input.txt

done



