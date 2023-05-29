mkdir -p tests
mkdir -p tests/small
mkdir -p tests/large

echo "------------------SMALL TEST CASE------------------"
if [ "$1" == "S" ] || [ "$2" == "S" ]; then
    echo "3" > tests/small/01
    echo "Generating test #1..."
    ./build/task/gen -full 4 -empty 2 -bubbles 4 -intensity 4 >> tests/small/01
    echo "Generating test #2..."
    ./build/task/gen -full 6 -empty 2 -bubbles 4 -intensity 4 >> tests/small/01
    echo "Generating test #3..."
    ./build/task/gen -full 10 -empty 2 -bubbles 5 -intensity 4 >> tests/small/01
else
    echo "Left tests as is"
fi

#./build/gen -full 20 -empty 3 -bubbles 4 -intensity 3 > tests/05 # Pretty fast but bad shuffle
# ./build/gen -full 4 -empty 3 -bubbles 4 -intensity 4 > tests/05 # Takes a longer time to compute

echo "------------------LARGE TEST CASE------------------"

if [ "$1" == "L" ] || [ "$2" == "L" ]; then
    echo "7" > tests/large/01

    echo "Generating test #1..."
    ./build/task/gen -full 200 -empty 100 -bubbles 20 -intensity 2 >> tests/large/01
    echo "Generating test #2..."
    # ./build/gen -full 2000 -empty 1000 -bubbles 2000 -intensity 2 >> tests/large/01
    ./build/task/gen -full 20 -empty 3 -bubbles 4 -intensity 4 >> tests/large/01
    echo "Generating test #3..."
    ./build/task/gen -full 100 -empty 15 -bubbles 4 -intensity 5 >> tests/large/01
    echo "Generating test #4..."
    ./build/task/gen -full 1000 -empty 200 -bubbles 30 -intensity 6 >> tests/large/01
    echo "Generating test #5..."
    ./build/task/gen -full 1000 -empty 100 -bubbles 20 -crop 50 -intensity 7 >> tests/large/01
    echo "Generating test #6..."
    ./build/task/gen -full 1000 -empty 100 -bubbles 150 -intensity 5 >> tests/large/01
    echo "Generating test #7..."
    ./build/task/gen -full 1000 -empty 100 -bubbles 220 -intensity 6 >> tests/large/01
else
    echo "Left tests as is"
fi
echo "------------------------DONE-----------------------"

mkdir -p solution
mkdir -p solution/small
mkdir -p solution/large

# echo "-------------------RUNNING SMALL-------------------"
# ./build/task/solutions/special < tests/small/01 > solution/small/01
# echo "-------------------RUNNING LARGE-------------------"
# ./build/task/solutions/special < tests/large/01 > solution/large/01
# echo "------------------------DONE-----------------------"

echo "-------------------JUDGING SMALL-------------------"
./build/task/checker tests/small/01 solution/small/01 package/small/01.a
echo "-------------------JUDGING LARGE-------------------"
./build/task/checker tests/large/01 solution/large/01 package/large/01.a
echo "------------------------DONE-----------------------"