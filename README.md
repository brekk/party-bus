
# party-bus

A conditional and tagged logger designed to be a near drop-in replacement for `IO.pTrace`


[![Madlib Project Badge](https://img.shields.io/badge/madlib-purple?logo=github&logoSize=auto)](//github.com/madlib-lang/madlib) <!-- $MADLIB.projectBadge -->
[![PartyBus v0.3.3](https://img.shields.io/badge/v0.3.3-purple?label=version)](//github.com/brekk/party-bus) <!-- $MADLIB.json.version -->

---

## Why use PartyBus?

PartyBus is a powerful and useful tool for empowering logging (or truly, any side effect that takes the form `String -> a -> a`, but for the porpoises 🐬 of this document, we'll focus on the most common cases).

It has several features which make it better than a simple `IO.pTrace`. All of these features are highly configurable, and we'll go into more detail below:

 1. Behavior is conditional and controlled via an environment variable, `DEBUG`. This expresses which tags are matched. Without this environment variable no calls will take place, and the program will behave as normally, sans logging.
 2. Logging can be moved to a log, `DEBUG_LOG` allows you to specify writing to a specific log file, e.g. `DEBUG_LOG="./path/to/logfile"`. This is optional. Lines written here will be in the form of JSONL by default.
 3. Loggers can be highly customized, but out-of-the-box PartyBus ships with a color-coded, date-annotated `message value` output
 4. There's an independent tool called `clown-car` which will automatically generate a standardized Log file from a sugar syntax

### TL;DR

> I'm unable to pay attention for contiguous blocks of time, sell it fast!

`import PartyBus from "PartyBus"`

 1. Define a tag. `tag = Party.tag("pizzeria")`
 2. Wrap the tag with an `env` wrapper to get an environment aware logger. `pizzeria = Party.env(tag)`
 3. Use the wrapped logger for fun and profit: `pizzeria("information", {goes: {right: "here"}})`
 4. Run the program that contains those loggers with a `DEBUG` environment variable: `DEBUG="*" madlib run src/MyProgram.mad`
 5. Tune the logging by expressing more complex tag queries, e.g. `DEBUG="!a:b:c,a:b:*,!a,!*"` would enable all `a:b` loggers and under, and disable all others.
 6. Optionally use the `DEBUG_LOG` parameter to allow for logging to a file in addition to logging to console

If you prefer, you can also just use [clown-car](//github.com/brekk/clown-car) to get 1-3 for free.

## Enabling and filtering logging

`DEBUG` is the environment variable that `party-bus` listens for by default. It uses `freitag` under the hood, which allows for expressing ad-hoc nested chains of strings.

Setting `DEBUG` defines the tag queries that PartyBus matches. These should be defined in decreasing specificity, as the first matcher wins.


### Matching any tag

Tag queries with the wildcard value: `*`, e.g. `DEBUG="*"`, it will enable matching all tags.
This can be nested underneath a container value: `a:*` would match `a` and any nested `a` tags, e.g. `a:b`
This can be nested multiple times, but the wildcard must be at the end of the list `a:b:*` is valid, `a:*:b` is not.

### Disabling a tag query

A `!` at the beginning of a tag query will disable matching it. `DEBUG="!a:b:c,*` would express "match everything except `a:b:c`" (remember we want to express thing with higher specificity first).

`!*` is a "match nothing" query. You mostly wouldn't want to use it by itself, as you can also just not define `DEBUG` entirely.

### Combining tag queries

Remember to express tag queries in decreasing specificity! `DEBUG="a:b:c:d,!a:b,a,!*"` would match `a:b:c:d` and anything under `a` that isn't also in `a:b`.

## Logging to a file

`DEBUG_LOG` is the environment variable that PartyBus listens for to enable writing the logs to a file. If it's not present, with the default configuration PartyBus will log only to console / `IO`. If `DEBUG` is not present, `DEBUG_LOG` has no bearing on behavior.

`DEBUG="*" DEBUG_LOG="./program.log" madlib run src/Program.mad` would enable all loggers, and it would write to `./program.log`

## Configuration

PartyBus is designed to work without much configuration out-of-the-box, but has easy interfaces which allow you to customize much of the behavior to your application if you need. These configuration details include when-to-fire, how-to-decorate-messages, how-to-decorate-values.

```madlib
alias Config = {
  change :: a -> b,
  check :: PartyPredicate a,
  decorate :: Decorator a,
  effect :: SideEffect b,
  mirror :: Mirror a b,
  seed :: String,
}
```

### Customizing your instance

In your module, if you want to, you can import `Config` to annotate your configuration
```madlib
import PB from "PartyBus"

MY_CONFIG :: PB.Config
MY_CONFIG = {
  ...PB.DATED_CONFIG,
  effect: (k, v) => IO.pTrace("x", v)
}
const env = PB.bus(MY_CONFIG)
```

At this point, if you want to know even more, you should probably look at [the source](//github.com/brekk/party-bus/blob/main/src/PartyBus.mad), as most of PartyBus is really just one big partially applied function.

## Using a standardized approach

PartyBus is intended to be very flexible and highly customizable. However, many libraries are using [clown-car](//github.com/brekk/clown-car) as a means of maintaining a logging harness file which is represented by a simpler syntax and regenerated as needed. With it, you can create a file like this:

```
pizzeria
- register
- register:detail
- menu
- menu:special
- menu:outage
```

and save it as something like `my-program.clown-car` (name doesn't really matter, but by convention a `whatever.clown-car` is used). Then when `clowncar` is run against the file, it will generate something like:
```
import Party from "@/PartyBus"

export tags = {
  pizzeria: Party.tag("pizzeria"),
  register: Party.tagWithScope("pizzeria", ["register"]),
  registerDetail: Party.tagWithScope("pizzeria", ["register", "detail"]),
  menu: Party.tagWithScope("pizzeria", ["menu"]),
  menuSpecial: Party.tagWithScope("pizzeria", ["menu", "special"]),
  menuOutage: Party.tagWithScope("pizzeria", ["menu", "outage"]),
}
export env = {
  pizzeria: Party.env(tags.pizzeria),
  register: Party.env(tags.register),
  registerDetail: Party.env(tags.registerDetail),
  menu: Party.env(tags.menu),
  menuSpecial: Party.env(tags.menuSpecial),
  menuOutage: Party.env(tags.menuOutage),
}
export tapped = {
  pizzeria: Party.tap(env.pizzeria),
  register: Party.tap(env.register),
  registerDetail: Party.tap(env.registerDetail),
  menu: Party.tap(env.menu),
  menuSpecial: Party.tap(env.menuSpecial),
  menuOutage: Party.tap(env.menuOutage),
}
```

This allows us to easily import this generated fixture `import Log from "./path/to/generated/file.mad"` and use the exported `env` value to have loggers which are automatically wired to `DEBUG`.
