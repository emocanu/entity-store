## Here are my comments

The SimpleStore relies on an unordered_map. Inserts are fast and also updates, removes.  
Queries have linear performance, that is why on the Store I relied on additional structures.  
Here, multimaps are storing pairs of ```{title, id}``` and ```{timestamp, id}```, making Queries much faster.  
These things can be seen also in the simple benchmarks I've implemented.  

##### Excerpts from results:  
```
Store
init_store = 5.85725 seconds.
Store::query_title = 0.00011962 seconds.
update_titles_test = 0.00157718 seconds.
update_timestamp_test = 0.000577573 seconds.


Simple Store
init_store = 3.69986 seconds.
SimpleStore::query_title = 0.0309539 seconds
update_titles_test = 0.0646574 seconds.
update_timestamp_test = 0.0645644 seconds.

```
  
### Why did I not work so happily at step 3 and 4?  

#### Step 3 - nothing implemented
I do not know the constraints of the project but I can imagine copying a whole Store being time and memory consuming.  
I would prefer a different approach for commits and reverts. I imagine a sort of structure which keeps the needed operations and can launch them at will, e.g. an entry in that structure translates to a method call in our store.  
I did not analyse the store API for revertness but I think is doable, for every action shall be an opposite such that the store rolls back in its initial state.  
##### To answer your question, we have two situations here, when the copy approach is used, we simply merge multiple children, when the only one store approach is used, we use exclusive access to the one and only store (mutex, condition variable, etc)  

#### Step 4  
I cannot see a better performance than using a multimap in which regards the child stores and range query improvements.  
  
  
  
  
## ********************** original document from here on **********************  


# Entity Store Example Project

Welcome to our sample project!

We would love to see how you code, and more importantly, how you think. The project is inspired by real world issues we’re facing, and we hope you’ll find it interesting.

Please keep a few things in mind:
- Keep things simple – we value your time and won’t nitpick unimportant details. We’re mainly interested in overall API design, correct memory management, algorithmic correctness, and performance considerations.
- Complete as much of these assignments as you’re comfortable doing at a high level of quality. If you don’t have enough time, it’s better to send in a partial implementation that’s great than a “complete” implementation full of bugs.
- Please add code comments in places where you had to make interesting decisions or design tradeoffs, or in places where you want to make your assumptions explicit.

If you have any questions regarding this assignment, don’t hesitate to contact us via jobs@culturedcode.com before sending in your application.


## Problem and Motivation

Our application uses domain-specific entities like `Todo`s:

```

  struct Todo {
    int64_t id;
    std::string title;
    std::string description;
    double timestamp;
    // and many more in the shipping app
  };

```

These need to be stored, retrieved, deleted, and queried. Your task is to implement an in-memory store component to provide this functionality.

Of course, in our actual application we also need to persist and sync these entities. This is far beyond the scope of this exercise and so you shouldn’t complicate the project with these features.


## Programming Language

Ideally, the project would be written in C++, as that’s the language you’ll be using when working here. However, if you’re still new to C++, you may select another performant, manual memory managed language such as C, or Rust.

You can put your code into the `main.cpp` file, compiled by `make`, or you can create a new Xcode project. Whatever you do, please make sure we can compile and run your solution. Please also show off your implemented functionality via interesting examples in the `main()` function body.


## Assignment Part 1 – A Basic Store

Implement a `Store` component which is able to take care of entities identified by a unique `int64_t id` and represented as a map of properties. Here’s an API sketch in pseudo language:

```

  Store store;

  // Insert a new entity
  store.insert(id = 2133, properties = { "title" => "Buy Milk", 
                                         "description" => "made of almonds!", 
                                         "timestamp" => 2392348.12233 })

  // Update only specified properties
  store.update(id = 2133, properties = { "title" => "Buy Chocolate" })

  // Retrieve properties of an entity
  store.get(id = 2133)
  // returns { "title" => "Buy Chocolate",
               "description" => "made of almonds!",
               "timestamp" => 2392348.12233 }

  // Remove an entity with given id
  store.remove(id = 2133)

```

Make sure your implementation correctly and efficiently handles multiple consecutive inserts, updates, and removals.


## Assignment Part 2 – Queries

To make our `Store` more useful, we’re going to support two simple kinds of queries:

```

  store.query("title" => "Buy Milk")
  // returns a set of ids where the value of the "title" property is equal to "Buy Milk"

  store.range_query("timestamp" => (1000, 1300))
  // returns a set of ids where the value of the "timestamp" property is in the given range

```


## Assignment Part 3 – Child Stores

Sometimes it’s very useful to have nested transactions – so we can commit (or throw away) changes in bulk. Let’s support this. We are going to represent such pending transactions by using a “child store” concept. Here’s how it could work:

```

  Store store;
  store.insert(id = 2133, properties = { "title" => "Buy Milk", 
                                         "description" => "made of almonds!" })

  // 1. Create a child store

  Store child = store.create_child();


  // 2. Use it to insert, get, update, delete, and query entities

  child.get(id = 2133) // returns { "title" => "Buy Milk", "description" => "made of almonds!" }

  child.update(id = 2133, { "title" => "Buy Cream" })

  child.get(id = 2133) // returns { "title" => "Buy Cream", "description" => "made of almonds!" }

  store.get(id = 2133) // returns { "title" => "Buy Milk", "description" => "made of almonds!" }

  child.query("title" => "Buy Cream") // returns { 2133 }


  // 3. Remove the child store and commit its changes to the parent

  child.commit()

  store.get(id = 2133) // returns { "title" => "Buy Cream", "description" => "made of almonds!" }

```

The design space for the implementation of child stores is large, and the decisions you make might also affect your implementation of Assigments 1 and 2. Please comment on these decisions in your code.

Please also elaborate in a comment in your code what would happen in a situation where the main store has two children (i.e. siblings), both of which contain pending changes to the same entity, and one of them is committed to the parent store. What, in your opinion, are good ways to deal with this situation?


## Assignment Part 4 – Performance Considerations

Please brainstorm in a comment in your code how you could improve the performance of your `range_query` implementation. You should take child stores from Part 3 into account.
