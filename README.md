
# party-bus

A conditional and tagged logger designed to be a near drop-in replacement for `IO.pTrace`


[![Madlib Project Badge](https://img.shields.io/badge/madlib-purple?logo=github&logoSize=auto)](//github.com/madlib-lang/madlib) <!-- $MADLIB.projectBadge -->
[![PartyBus v0.3.2](https://img.shields.io/badge/v0.3.2-purple?label=version)](//github.com/brekk/party-bus) <!-- $MADLIB.json.version -->

---

PartyBus is a powerful and useful tool for empowering logging.

It has several features which make it better than a simple `IO.pTrace`:

 1. Logging is conditional and controlled via an environment variable, `DEBUG`. This value can express many different loggers, as well as match wildcard expressions. `DEBUG="!a:b:c,a:b:*,!*"` would allow you to enable all `a:b:*` loggers except for `a:b:c`, and no other loggers. It's important that these values be most specific matchers first, as they block successive matches.
 2. Additionally, `DEBUG_LOG="./path/to/logfile"` will afford writing to a specific log file.
 2. By default, each logger will automatically be color-coded, to help disambiguation between loggers. It also captures the current time.
