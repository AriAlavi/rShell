INPUTS=(
    "echo ping; echo ping"
)
OUTPUTS=(
    "pingping"
)

LENGTH=${#INPUTS[*]}
LENGTH="$(($LENGTH-1))"
for i in $LENGTH
do
    INPUT="${INPUTS[${i}]}"
    EXPECTEDOUTPUT="${OUTPUTS[${i}]}"
    OUTPUT=$(../rshell "$INPUT")
    OUTPUT=${OUTPUT//$'\n'/}
    echo "Input:" $INPUT
    echo "Output:" "${OUTPUT}"
    echo "EXPECTEDOUTPUT:" "${EXPECTEDOUTPUT}"


    if [ "${OUTPUT}" = "${EXPECTEDOUTPUT}" ]
    then
        echo "Test passed"
    else
        echo "Test failed"
    fi

done

