window.addEventListener("load",init);
function init(){
    
    clearAll();
    loadId();
    showTotal();
    bindEvents();  
}

function clearAll(){

    /* this function clears the contents of the form except the ID (since ID is auto generated)*/
    
}

let auto = autoGen();

function loadId(){
    /* this function automatically sets the value of ID */
    document.querySelector('#id').innerText = auto.next().value;
    

}

function showTotal(){
    /* this function populates the values of #total, #mark and #unmark ids of the form */
    
}

function bindEvents(){
    
    document.querySelector('#remove').addEventListener('click',deleteRecords);
    document.querySelector('#add').addEventListener('click',addRecord);
    document.querySelector('#update').addEventListener('click',updateRecord)
    document.querySelector('#exchange').addEventListener('change',getExchangerate)
}

function deleteRecords(){
    /* this function deletes the selected record from itemOperations and prints the table using the function printTable*/
   
}




function addRecord(){
    /* this function adds a new record in itemOperations and then calls printRecord(). showTotal(), loadId() and clearAll()*/
    
}
function edit(){
    /*this function fills (calls fillFields()) the form with the values of the item to edit after searching it in items */ 
    
     
}
function fillFields(itemObject){
    /*this function fills the form with the details of itemObject*/
}
function createIcon(className,fn, id){
 /* this function creates icons for edit and trash for each record in the table*/
    // <i class="fas fa-trash"></i>
    // <i class="fas fa-edit"></i>
    var iTag = document.createElement("i");
    iTag.className = className;
    iTag.addEventListener('click',fn);
    iTag.setAttribute("data-itemid", id) ;

    return iTag;
}


function updateRecord(){
    /*this function updates the record that is edited and then prints the table using printTable()*/
    
}

function trash(){
    /*this function toggles the color of the row when its trash button is selected and updates the marked and unmarked fields */
    let id = this.getAttribute('data-itemid');
    itemOperations.markUnMark(id);
    showTotal();
    let tr = this.parentNode.parentNode;
    tr.classList.toggle('alert-danger');
    console.log("I am Trash ",this.getAttribute('data-itemid'))
}

function printTable(items){
   /* this function calls printRecord for each item of items and then calls the showTotal function*/
}
function printRecord(item){
    var tbody = document.querySelector('#items');
    var tr = tbody.insertRow();
    var index = 0;
    for(let key in item){
        if(key=='isMarked'){
            continue;
        }
        let cell = tr.insertCell(index);
        cell.innerText = item[key] ;
        index++;
    }
    var lastTD = tr.insertCell(index);
    lastTD.appendChild(createIcon('fas fa-trash mr-2',trash,item.id));
    lastTD.appendChild(createIcon('fas fa-edit',edit,item.id));
}

function getExchangerate(){
    /* this function makes an AJAX call to http://apilayer.net/api/live to fetch and display the exchange rate for the currency selected*/
    
}