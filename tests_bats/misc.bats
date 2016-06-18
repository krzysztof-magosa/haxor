@test "misc: preserving saved registers in prol/epil" {
    hc prol_epil_01.hax
    run hvm prol_epil_01.hax.e

    [ "$output" = "0|1|2|3|4|5|6|7|8|9" ]
}

@test "misc: reference to non existing_label" {
    run hc reference_to_non_existing_label.hax

    [ "$output" = "Reference to not existing label 'nonExistingLabel', at location 3.5-20" ]
}

@test "misc: big value passed to li" {
    hc li.hax
    run hvm li.hax.e

    [ "$output" = "1125899906842624" ]
}
