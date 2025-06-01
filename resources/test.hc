function test_func_a:
    say "I am function A!"
end

function test_func_b:
    say "Before calling function A!"
    test_func_a
    say "Before called function B!"
end

start:
    say "Hello HerCode!!"
    test_func_b
end