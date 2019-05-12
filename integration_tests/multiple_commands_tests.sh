INPUTS=(
    "echo ping; echo ping"
    "ech crash || echo ping; echo ping"
    "echo ping && echo ping"
    "ech crash || echo ping && echo ping"
    "ech crash || ech crash || echo ping && echo ping"
    "echo ping && echo ping || echo ping"
    "echo ping || echo ping && echo ping"
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
        echo "Test failed"
    fi
    printf "\n"

done
