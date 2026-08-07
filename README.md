EliteXcoin Core
=====================================

https://elitexcoin.io

What is EliteXcoin?
----------------

EliteXcoin (EXC) is a decentralised, 100%-proof-of-stake digital currency
forked from Blackcoin's PoS 3.1 (PoSV3, BPoS) codebase, itself built on
Bitcoin Core. See [ops/GENESIS.md](../ops/GENESIS.md) for the coin's
consensus parameters, premine, and staking reward formula.

What is EliteXcoin Core?
----------------

EliteXcoin Core is the name of the open source software which enables use
of the EliteXcoin protocol. It connects to the EliteXcoin peer-to-peer
network to download and fully validate blocks and transactions. It also
includes a wallet and graphical user interface, which can be optionally
built.

License
-------

EliteXcoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is regularly built (see `doc/build-*.md` for instructions) and tested.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md)
and useful hints for developers can be found in [doc/developer-notes.md](doc/developer-notes.md).

Testing
-------

Testing and code review might be the bottleneck for development. Please help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The CI (Continuous Integration) systems build every pull request for Windows, Linux, and macOS,
and run unit/sanity tests automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Acknowledgements
------------

EliteXcoin Core is a fork of [Blackcoin More](https://github.com/CoinBlack/blackcoin-more),
which is itself built on [Bitcoin Core](https://github.com/bitcoin/bitcoin). Thanks to both
projects' contributors for the codebase this builds on.
