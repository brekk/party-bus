# party-bus

Currently folks can use `IO.pTrace` or `IO.trace` for a string logger.
It is synchronous and functions as identity plus a side-effect.
It takes the form of a message and any value, i.e.

```
IO.pTrace("message", anyValue)
```

In order to make this library effective, we should make a synchronous API which can function as a
drop-in replacement for the common IO behavior.

However, we also have need for the following modifications:

 1. Rather than ad-hoc / one-off messages ( i.e. `IO.pTrace("output", x)` ), we should support tags.
    Tags are known before the logger is used, and provide a means of segmenting messages.
    Tags can be strings (i.e. "parser" ) or be scoped to be more specific ( i.e. "parser:json:string" )
    However, tags are an addendum to the message / value form presented above, i.e.

    ```
    parseLog = PartyBus.taggedLog("parser")
    parseLog("message", anyValue)

    parseJLog = PartyBus.taggedLogWithScope("parser", ["json", "string"])
    parseJLog("message", anyValue)
    ```
 2. We shall support other kinds of side-effectful behavior, not just logging.

    ```
    taggedLog = PartyBus.tagged(IO.putLine)
    ```

 3. Because we're invoking a side-effect, we should provide a means of manipulating the value as it
    goes through the bus:

    ```
    log = PartyBus.taggedLogWithScope("sell", ["pastry"])
    // modify the log to show only the price when called
    PartyBus.transform(log, .price, "The price of the pastry is")
    ```

 4. We shall support conditional logging.
    
    ```
    log = PartyBus.taggedLogWithScope("sell", ["pastry"])
    PartyBus.callWhen(pipe(.price, lt(40)), log, "The price of pastry is under $40")
    ```

    Ostensibly we should be able to both manipulate the value and invoke the call conditionally:

    ```
    log = PartyBus.taggedLogWithScope("sell", ["pastry"])
    PartyBus.transformWhen(pipe(.price, lt(40)), .name, log, "The name of pastries under $40")
    ```

