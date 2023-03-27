/**
 * refresh the contact list
 */
async function fillBuckets() {

    // empty the contact html list
    let list = document.getElementById('buckets');
    list.innerHTML = "";
  
    // query all buckets from the database
    let resp = await fetch('buckets');
    let buckets = await resp.json();
  
    // for each bucket in the array, append a list item with the bucket name
    let html = "";
    for (i = 0; i < buckets.length; i++) {
        html += `<li>${buckets[i].name}</li>`
    }
    list.innerHTML = html;

    console.log(`There is ${buckets.length} buckets in the list`);
  }
  
  fillBuckets();
  