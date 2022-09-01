let counter;

function* autoGen(){
    counter = 1;
    while(true){
    yield counter;
    counter++;
    }
    }
    