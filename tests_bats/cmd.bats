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

@test "cmd: beq" {
    hc beq.hax
    run hvm beq.hax.e

    [ "$status" = "100" ]
}

@test "cmd: beql" {
    hc beql.hax
    run hvm beql.hax.e

    [ "$output" = "12" ]
}

@test "cmd: bne" {
    hc bne.hax
    run hvm bne.hax.e

    [ "$status" = "100" ]
}

@test "cmd: bnel" {
    hc bnel.hax
    run hvm bnel.hax.e

    [ "$output" = "12" ]
}

@test "cmd: j" {
    hc j.hax
    run hvm j.hax.e

    [ "$status" = "10" ]
}

@test "cmd: jr" {
    hc jr.hax
    run hvm jr.hax.e

    [ "$status" = "20" ]
}

@test "cmd: jal" {
    hc jal.hax
    run hvm jal.hax.e

    [ "$output" = "20" ]
}

@test "cmd: jalr" {
    hc jalr.hax
    run hvm jalr.hax.e

    [ "$output" = "20" ]
}

@test "cmd: push" {
    hc push.hax
    run hvm push.hax.e

    [ "$status" = "50" ]
}

@test "cmd: pushi" {
    hc pushi.hax
    run hvm pushi.hax.e

    [ "$status" = "50" ]
}

@test "cmd: pushm" {
    hc pushm.hax
    run hvm pushm.hax.e

    [ "$status" = "33" ]
}

@test "cmd: pop" {
    hc pop.hax
    run hvm pop.hax.e

    [ "$status" = "150" ]
}

@test "cmd: popm" {
    hc popm.hax
    run hvm popm.hax.e

    [ "$status" = "166" ]
}

@test "cmd: move" {
    hc move.hax
    run hvm move.hax.e

    [ "$status" = "177" ]
}

@test "cmd: clear" {
    hc clear.hax
    run hvm clear.hax.e

    [ "$status" = "0" ]
}

#@test "cmd: not" {
#    hc not.hax
#    run hvm not.hax.e
#
#    [ "$status" = "0" ]
#}

@test "cmd: ret" {
    hc ret.hax
    run hvm ret.hax.e

    [ "$output" = "12" ]
}

@test "cmd: b" {
    hc b.hax
    run hvm b.hax.e

    [ "$status" = "155" ]
}

@test "cmd: bal" {
    hc bal.hax
    run hvm bal.hax.e

    [ "$output" = "20" ]
}

# bgt
# blt
# bge
# ble
# blez
# bgtz
# beqz

@test "cmd: li" {
    hc li.hax
    run hvm li.hax.e

    [ "$output" = "1125899906842624" ]
}

@test "cmd: la" {
    hc la.hax
    run hvm la.hax.e

    [ "$output" = "hello" ]
}

@test "cmd: prol" {
    hc prol.hax
    run hvm prol.hax.e

    [ "$output" = "9|8|7|6|5|4|3|2|1|0|987|156|" ]

    # missing: $fp=$sp
    # missing: $sp-=imm
}

# epil

@test "cmd: resw" {
    hc resw.hax
    run hvm resw.hax.e

    [ "$output" = "80" ]
}
