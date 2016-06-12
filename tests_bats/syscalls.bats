@test "syscall: print" {
    hc syscall_01h.hax
    run hvm syscall_01h.hax.e

    [ "$output" = "test_string" ]
}

@test "syscall: printi" {
    hc syscall_02h.hax
    run hvm syscall_02h.hax.e

    [ "$output" = "165" ]
}

@test "syscall: scan" {
    hc syscall_03h.hax
    result=$(echo "test_string" | hvm syscall_03h.hax.e)

    [ "$result" = "test_string" ]
}

@test "syscall: scani" {
    hc syscall_04h.hax
    result=$(echo "290" | hvm syscall_04h.hax.e)

    [ "$result" = "290" ]
}

@test "syscall: rand" {
    hc syscall_05h.hax
    run hvm syscall_05h.hax.e

    [ "$output" -ge 0 ]
    [ "$output" -le 100 ]
}

@test "syscall: sleep" {
    hc syscall_06h.hax

    started=$SECONDS
    run hvm syscall_06h.hax.e
    delay=$[SECONDS-started]

    [ $delay -ge 2 ]
    [ $delay -le 3 ]
}
