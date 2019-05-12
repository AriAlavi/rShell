INPUTS=(
    "echo ping; echo ping; exit; echo ping;"
    "echo ping && echo ping && exit && echo ping"
    "echo ping || exit && echo ping"
    "echo ping || echo ping && echo ping && exit"
    "ech ping || echo ping || ech ping && echo ping || exit || echo ping && exit"

)


LENGTH=${#INPUTS[*]}
LENGTH="$(($LENGTH-1))"
for i in $(seq 0 $LENGTH)
do
    INPUT="${INPUTS[${i}]}"
    EXPECTEDOUTPUT="pingping"
    OUTPUT=$(../rshell "$INPUT")
    OUTPUT=${OUTPUT//$'\n'/}
    echo "Input:" $INPUT
    echo "Output:" "${OUTPUT}"

    if [ "${OUTPUT}" = "${EXPECTEDOUTPUT}" ]
    then
        echo "Test passed"
    else
        printf "\n_____________________\n\nTest failed\nTest failed\nTest failed\n_____________________\n"
        
    fi
    printf "\n"

done

