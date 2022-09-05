window.addEventListener("load", init);

function init() {

    clearAll();
    loadId();
    showTotal();
    bindEvents();
}

function clearAll() {
    /* this function clears the contents of the form except the ID (since ID is auto generated)*/
    const inputs = document.querySelectorAll('.form-control');
    inputs.forEach(input => {
        if (input.id !== "id") {
            input.value = "";
            switch (input.id) {
                case ("name"):
                    input.placeholder = "Type Name Here";
                    break;
                case ("price"):
                    input.placeholder = "Select Price in $ Here";
                    break;
                case ("desc"):
                    input.placeholder = "Type Desc Here";
                    break;
                case ("url"):
                    input.placeholder = "http://sample.com";
                    break;
            }
        }
    });
}

let auto = autoGen();
let selectedItem = null;

function loadId() {
    /* this function automatically sets the value of ID */
    document.querySelector('#id').innerText = auto.next().value;


}

function showTotal() {
    /* this function populates the values of #total, #mark and #unmark ids of the form */
    document.querySelector('#total').innerHTML = itemOperations.items.length.toString();
    document.querySelector('#mark').innerHTML = itemOperations.countTotalMarked().toString();
    document.querySelector('#unmark').innerHTML =
        (itemOperations.items.length - itemOperations.countTotalMarked()).toString();
}

function bindEvents() {

    document.querySelector('#remove').addEventListener('click', deleteRecords);
    document.querySelector('#add').addEventListener('click', addRecord);
    document.querySelector('#update').addEventListener('click', updateRecord)
    document.querySelector('#exchange').addEventListener('change', getExchangerate)

    document.querySelector('#save').addEventListener('click', saveLocally)
    document.querySelector('#load').addEventListener('click', loadFromLocal)

    document.querySelector('#load-remote').addEventListener('click', loadFromServer);
    document.querySelector('#save-remote').addEventListener('click', saveToServer);
}

function deleteRecords() {
    /* this function deletes the selected record from itemOperations and prints the table using the function printTable*/
    document.querySelector('#items').innerHTML = "";
    itemOperations.remove();
    printTable(itemOperations.items);
}

function addRecord() {
    /* this function adds a new record in itemOperations and then calls printRecord(). showTotal(), loadId() and clearAll()*/
    const newItem = new Item(
        document.querySelector('#id').innerHTML,
        document.querySelector('#name').value,
        document.querySelector('#price').value,
        document.querySelector('#desc').value,
        document.querySelector('#color').value,
        document.querySelector('#url').value,
    );
    itemOperations.add(newItem);
    printRecord(newItem);
    showTotal();
    loadId();
    clearAll();
}

function edit() {
    /*this function fills (calls fillFields()) the form with the values of the item to edit after searching it in items */
    let id = this.getAttribute('data-itemid');
    let item = itemOperations.search(id);
    if (item !== null) {
        fillFields(item);
        selectedItem = item;
    }
}

function fillFields(itemObject) {
    /*this function fills the form with the details of itemObject*/
    const inputs = document.querySelectorAll('.form-control');
    inputs.forEach(input => {
        input.value = "";
        switch (input.id) {
            case ("id"):
                input.innerHTML = itemObject.id;
                break;
            case ("name"):
                input.value = itemObject.name;
                break;
            case ("price"):
                input.value = itemObject.price;
                break;
            case ("desc"):
                input.value = itemObject.desc;
                break;
            case ("color"):
                input.value = itemObject.color;
                break;
            case ("url"):
                input.value = itemObject.url;
                break;
        }
    });
}

function createIcon(className, fn, id) {
    /* this function creates icons for edit and trash for each record in the table*/
    // <i class="fas fa-trash"></i>
    // <i class="fas fa-edit"></i>
    var iTag = document.createElement("i");
    iTag.className = className;
    iTag.addEventListener('click', fn);
    iTag.setAttribute("data-itemid", id);

    return iTag;
}


function updateRecord() {
    /*this function updates the record that is edited and then prints the table using printTable()*/
    if (selectedItem != null) {
        document.querySelector('#items').innerHTML = "";

        selectedItem.id = document.querySelector('#id').innerHTML;
        selectedItem.name = document.querySelector('#name').value;
        selectedItem.price = document.querySelector('#price').value;
        selectedItem.desc = document.querySelector('#desc').value;
        selectedItem.color = document.querySelector('#color').value;
        selectedItem.url = document.querySelector('#url').value;

        printTable(itemOperations.items);
        selectedItem = null;
        loadId();
        clearAll();
    }
}

function trash() {
    /*this function toggles the color of the row when its trash button is selected and updates the marked and unmarked fields */
    let id = this.getAttribute('data-itemid');
    itemOperations.markUnMark(id);
    showTotal();
    let tr = this.parentNode.parentNode;
    tr.classList.toggle('alert-danger');
    console.log("I am Trash ", this.getAttribute('data-itemid'))
}

function printTable(items) {
    /* this function calls printRecord for each item of items and then calls the showTotal function*/
    items.forEach((item) => {
        printRecord(item);
    });
    showTotal();
}

function printRecord(item) {
    var tbody = document.querySelector('#items');
    var tr = tbody.insertRow();
    var index = 0;
    for (let key in item) {
        if (key == 'isMarked') {
            continue;
        }
        let cell = tr.insertCell(index);
        cell.innerText = item[key];
        index++;
    }
    var lastTD = tr.insertCell(index);
    lastTD.appendChild(createIcon('fas fa-trash mr-2', trash, item.id));
    lastTD.appendChild(createIcon('fas fa-edit', edit, item.id));
}

function getExchangerate() {
    /* this function makes an AJAX call to http://apilayer.net/api/live to fetch and display the exchange rate for the currency selected*/
    const amount = document.querySelector("#price").value;
    if (amount.trim() !== "") {
        const exchange = document.querySelector("#exchange").value;

        const makeHeaders = new Headers();
        makeHeaders.append("apikey", "H9NESMGiV6oUsuPsQTw7wORN1kxZwylR");

        const requestOptions = {
            method: 'GET',
            redirect: 'follow',
            mode: 'cors',
            headers: makeHeaders,
        };

        fetch(`https://api.apilayer.com/currency_data/convert?to=${exchange}&from=USD&amount=${amount}`, requestOptions)
            .then(resp => {
                if (!resp.ok) {
                    throw Error("Response did not return OK");
                }
                return resp.json();
            })
            .then(result => { document.querySelector('#exrate').value = result.result;})
            .catch((e) => console.log("Caught error", e));
    }
}

function saveLocally() {
    const table = document.querySelector('#items').children;
    let toStore = [];

    if (table.length === 0) {
        alert("table is empty, no changes saved!")
        return;
    }
    if (localStorage.length !== 0) {
        if (!confirm("This will delete previous saved data!\nContinue?")) {
            return;
        }
    }

    for (let row of table) {
        let i = 0;
        let nextItem = new Item();
        // loop through cells to construct the corresponding item
        for (let cell of row.cells) {
            switch (i) {
                case 0:
                    nextItem.id = cell.innerText;
                    break;
                case 1:
                    nextItem.name = cell.innerText;
                    break;
                case 2:
                    nextItem.price = cell.innerText;
                    break;
                case 3:
                    nextItem.desc = cell.innerText;
                    break;
                case 4:
                    nextItem.color = cell.innerText;
                    break;
                case 5:
                    nextItem.url = cell.innerText;
                    break;
            }
            i++;
        }
        nextItem.isMarked = false;
        // Store the item as a JSON string and set its key to be the id of the item
        toStore.push(nextItem);
    }
    localStorage.setItem("stored", JSON.stringify(toStore));
}

function loadFromLocal() {
    if (document.querySelector('#items').children.length > 0) {
        if (!confirm("This will overwrite the current table!\nContinue?")) {
            return;
        }
    }

    itemOperations.items = [];
    let lastId = 0;

    // parse the stored JSON items and convert them into Item object
    let loaded = JSON.parse(localStorage.getItem("stored"));
    loaded.forEach((item) => {
        if (item.id > lastId) {lastId = item.id; }
        let nextItem = new Item(
            item.id,
            item.name,
            item.price,
            item.desc,
            item.color,
            item.url,
            item.isMarked
        )
        itemOperations.add(nextItem);
    });

    // print the loaded item objects
    document.querySelector('#items').innerHTML = "";
    printTable(itemOperations.items);

    // Update the autogenerated counter (utils.js) to match the highest id value in local storage
    counter = parseInt(lastId);
    loadId();
}

function saveToServer() {
    const table = document.querySelector('#items').children;
    if (table.length === 0) {
        alert("table is empty, no changes saved!")
        return;
    }
    if (!confirm("This will delete previous saved data!\nContinue?")) {
        return;
    }

    const makeDeleteHeaders = new Headers();
    const deleteRequestOptions = {
        method: 'DELETE',
        headers: makeDeleteHeaders,
    };

    fetch(`http://localhost:5000/items`, deleteRequestOptions)
        .then( (resp) => {
            if (!resp.ok) {
                throw Error("Deletion Response did not return OK");
            }
        })
        .then(() => {
            fetch(`http://localhost:5000/items`, {
                method: 'POST',
                headers: {
                    'Accept': 'application/json',
                    'Content-Type': 'application/json'
                },
                body: itemsAsJson(table),
            })
                .then((resp) => {
                    if (!resp.ok) {
                        throw Error("Post Response did not return OK");
                    }
                    return resp.statusText
                })
                .then(data => {
                    console.log(data);
                })
                .catch((e) => console.log("Caught error", e));
        })
        .catch((e) => console.log("Caught error", e));
}

function loadFromServer() {
    const makeHeaders = new Headers();
    const requestOptions = {
        method: 'GET',
        headers: makeHeaders,
    };

    fetch(`http://localhost:5000/items`, requestOptions)
        .then( (resp) => {
            if (!resp.ok) {
                throw Error("Response did not return OK");
            }
            return resp.json();
        })
        .then(result => {
            if (result.length !== 0) {
                itemOperations.items = [];
                let lastId = 0;

                // parse the stored JSON items and convert them into Item object
                result.forEach((item) => {
                    if (item.id > lastId) { lastId = item.id; }
                    let nextItem = new Item(
                        item.id,
                        item.name,
                        item.price,
                        item.description,
                        item.color,
                        item.url,
                        item.isMarked
                    )
                    itemOperations.add(nextItem);
                });

                // print the loaded item objects
                document.querySelector('#items').innerHTML = "";
                printTable(itemOperations.items);

                // Update the autogenerated counter (utils.js) to match the highest id value in local storage
                counter = parseInt(lastId);
                loadId();
            }
        })
        .catch((e) => console.log("Caught error", e));
}

function itemsAsJson(table) {
    let allItems = []

    for (let row of table) {
        let i = 0;
        let nextItem = new Item();
        // loop through cells to construct the corresponding item
        for (let cell of row.cells) {
            switch (i) {
                case 0:
                    nextItem.id = cell.innerText;
                    break;
                case 1:
                    nextItem.name = cell.innerText;
                    break;
                case 2:
                    nextItem.price = cell.innerText;
                    break;
                case 3:
                    nextItem.desc = cell.innerText;
                    break;
                case 4:
                    nextItem.color = cell.innerText;
                    break;
                case 5:
                    nextItem.url = cell.innerText;
                    break;
            }
            i++;
        }
        nextItem.isMarked = false;
        // Store the item as a JSON string and set its key to be the id of the item
        allItems.push(nextItem);
    }
    return JSON.stringify(allItems)
}