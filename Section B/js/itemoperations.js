const itemOperations = {
    items:[],
    add(itemObject){
        /* adds an item into the array items*/
        this.items.add(itemObject);
    },
    remove(){
         /* removes the item which has the "isMarked" field set to true*/
        this.items.forEach((item) => {
            if (item.isMarked) { this.items.remove(item) }
        })
   },
    search(id){
        /* searches the item with a given argument id */
        this.items.forEach((item) => {
            if (item.id === id) { return item; }
        })
    },
    markUnMark(id){
        /* toggle the isMarked field of the item with the given argument id*/
        this.items.forEach((item) => {
            if (item.id === id) { item.toggle(); }
        })
    },
   countTotalMarked(){
       /* counts the total number of marked items */
       let num = 0;
       this.items.forEach((item) => {
           if (item.isMarked) { num++; }
       })
       return num;
},
   
}