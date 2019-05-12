INPUTS=(
    "echo hello; #echo comment"
    "#echo comment"
    "echo bye && #echo comment"
    "echo testing; #echo comment; echo hello"
    'echo "ping && ping"'
)
OUTPUTS=(
    "hello"
    ""
    "bye"
    "testing"
    "ping && ping"
)

LENGTH=${#INPUTS[*]}
LENGTH="$(($LENGTH-1))"
for i in $(seq 0 $LENGTH)
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
    printf "\n"

done