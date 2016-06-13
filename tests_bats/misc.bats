@test "misc: preserving saved registers in prol/epil" {
    hc prol_epil_01.hax
    run hvm prol_epil_01.hax.e

    [ "$output" = "0|1|2|3|4|5|6|7|8|9" ]
}
