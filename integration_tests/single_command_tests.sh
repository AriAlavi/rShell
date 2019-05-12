echo "Running tests"
INPUTS=(
    "ping"
    "ping ping;"
    "ping ping ping"
    "ping ping ping ping"
    "ping ping ping ping ping ping"
    "ping ping ping ping ping ping ping"
    "ping ping ping ping ping ping ping ping"
)

for input in "${INPUTS[@]}"
do

    output=$(../rshell "echo \"${input}\"")

    newoutput="${output%\"}"
    newoutput="${newoutput#\"}"
    
    echo "Input:" $input
    echo "Output:" $newoutput

    

    if [ "${newoutput}" = "${input}" ]
    then
        echo "Test passed"
    else
        echo "Test failed"
    fi

done

