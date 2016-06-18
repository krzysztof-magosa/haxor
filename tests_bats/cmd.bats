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
