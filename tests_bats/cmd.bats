@test "cmd: add" {
    hc add.hax
    run hvm add.hax.e

    [ "$output" = "300" ]
}

@test "cmd: addi" {
    hc addi.hax
    run hvm addi.hax.e

    [ "$output" = "300" ]
}

@test "cmd: sub" {
    hc sub.hax
    run hvm sub.hax.e

    [ "$output" = "300" ]
}

@test "cmd: mult" {
    hc mult.hax
    run hvm mult.hax.e

    [ "$output" = "300" ]
}

@test "cmd: div" {
    hc div.hax
    run hvm div.hax.e

    [ "$output" = "300" ]
}

@test "cmd: mod" {
    hc mod.hax
    run hvm mod.hax.e

    [ "$output" = "300" ]
}

@test "cmd: lw" {
    hc lw.hax
    run hvm lw.hax.e

    [ "$output" = "300" ]
}

@test "cmd: sw" {
    hc sw.hax
    run hvm sw.hax.e

    [ "$output" = "300" ]
}

@test "cmd: lui" {
    hc lui.hax
    run hvm lui.hax.e

    [ "$output" = "858993459200" ]
}

@test "cmd: and" {
    hc and.hax
    run hvm and.hax.e

    [ "$output" = "17" ]
}

@test "cmd: andi" {
    hc andi.hax
    run hvm andi.hax.e

    [ "$output" = "17" ]
}

@test "cmd: or" {
    hc or.hax
    run hvm or.hax.e

    [ "$output" = "31" ]
}

@test "cmd: ori" {
    hc ori.hax
    run hvm ori.hax.e

    [ "$output" = "31" ]
}

@test "cmd: xor" {
    hc xor.hax
    run hvm xor.hax.e

    [ "$output" = "0110" ]
}

@test "cmd: nor" {
    hc nor.hax
    run hvm nor.hax.e

    [ "$output" = "-1-2-2-2" ]
}

@test "cmd: slt" {
    hc slt.hax
    run hvm slt.hax.e

    [ "$output" = "10" ]
}

@test "cmd: slti" {
    hc slti.hax
    run hvm slti.hax.e

    [ "$output" = "10" ]
}

@test "cmd: slli" {
    hc slli.hax
    run hvm slli.hax.e

    [ "$output" = "128" ]
}

@test "cmd: srli" {
    hc srli.hax
    run hvm srli.hax.e

    [ "$output" = "8" ]
}

@test "cmd: sll" {
    hc sll.hax
    run hvm sll.hax.e

    [ "$output" = "128" ]
}

@test "cmd: srl" {
    hc srl.hax
    run hvm srl.hax.e

    [ "$output" = "8" ]
}
