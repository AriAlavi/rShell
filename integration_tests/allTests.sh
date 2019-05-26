JSONS=""
# hey look! it's an actually useful integration test!!!
for json in *.json; do

    JSONS="$JSONS $json"

done

python3 test.py $JSONS