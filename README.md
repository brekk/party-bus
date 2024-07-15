# party-bus

# Usage

```mad
import Party from "PartyBus"

// Make a PartyBus!
// bus takes:
//   - a trace function: String -> a -> a
//   - an event ruleset: List Tag
//   - an event: Tag
//   - a subject: a

bus = Party.bus(IO.trace)
partybus = Party.bus(IO.pTrace)

// Inform your guests about rules of the party

invite = Party.bus(IO.trace, [
  // eat dance talk be-merry
  Party.allow("eating", []),
  Party.allow("dancing", ["rooms", "living-room"]),
  Party.allow("talking", []),
  Party.allow("playing", []),
  // don't do this stuff
  Party.avoid("singing"),
  Party.avoid("playing", ["tv"])
  Party.avoid("playing", ["guitar"])
  Party.avoid("eating", ["rooms", "kitchen"])
  Party.avoid("smoking", ["indoors"]),
])

// RSVP

// this bakes in the rules

try = Party.rsvp(invite)

// Stuff you _can't_ do at the party

// uncool keith - You know dancing only in the dancing:rooms:living-room
try(Party.event("dancing"), "I'm dancing in the kitchen")

// uncool singing sally - You know no singing!
try(Party.event("singing"), "I love singing and demanding attention!")

// uncool debbie downer - Keep the vibe up!
try(Party.event("downer"), "By the way... it's official...")

// Stuff you _can_ do at the party

danceInTheLivingRoom = try(Party.event("dancing", ["rooms", "living-room"]))
danceInTheLivingRoom("I love living room dancing and not dancing in other rooms!")
danceInTheLivingRoom("I too enjoy dancing in the appropriate room!")

try(Party.event("eating", ["rooms", "dining-room"]), "I enjoy eating in the appropriate room!")
try(Party.event("smoking", ["outside"]), "I am not smoking:indoors!")

```

events with signals and processes

- toggleable
- tagged
- nested
- auto-colors
- filter
- meta tooling?
  - capture timing?
