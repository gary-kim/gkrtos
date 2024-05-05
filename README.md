# GKRTOS: A Personal Real Time Operating System

[![builds.sr.ht status](https://builds.sr.ht/~gary-kim/gkrtos/commits/master/test.yml.svg)](https://builds.sr.ht/~gary-kim/gkrtos/commits/master/test.yml?)

* Project Home: [https://sr.ht/~gary-kim/gkrtos](https://sr.ht/~gary-kim/gkrtos)
* Source Code: [https://git.sr.ht/~gary-kim/gkrtos](https://git.sr.ht/~gary-kim/gkrtos)
* Development Mailing List: [https://lists.sr.ht/~gary-kim/public-inbox](https://lists.sr.ht/~gary-kim/public-inbox) ([~gary-kim/public-inbox@lists.sr.ht](mailto:~gary-kim/public-inbox@lists.sr.ht))
* TODO: [https://todo.sr.ht/~gary-kim/gkrtos](https://todo.sr.ht/~gary-kim/gkrtos)

### Building

```bash
mkdir tmp
cd tmp
cmake ..
make -j $(nproc)
```

### Current Stage

While the intention is to make this RTOS architecture agnostic in the future and
make it capable of running on many different microcontrollers. In the interest
of time, it is being designed directly for the RP2040 initially. This will later
be decoupled.

### Useful Resources

* ARMv6-M Reference Manual: https://documentation-service.arm.com/static/5f8ff05ef86e16515cdbf826
* RP2040 Datasheet: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
* RP2040 Hardware Design: https://datasheets.raspberrypi.com/rp2040/hardware-design-with-rp2040.pdf

### License

Licensed under [AGPL-3.0-or-later](./LICENSE), with usage of revised versions of
the Affero General Public License subject to Gary Kim (<https://garykim.dev>) as
an upgrade proxy under section 14 of the AGPL-3.0-or-later license.

