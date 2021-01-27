
use std::assert;
use std::env;

use aoc2018;

fn main() {
    let args: Vec<String> = env::args().collect();
    assert!(args.len() == 2);

    let day = args[1].parse::<u32>();
    let day = match day {
        Ok(day) => day,
        Err(e) => panic!("{}", e),
    };
    assert!(1 <= day && day <= 25);

    match day {
        1 => println!("Part 1: {}\nPart 2: {}",
                       aoc2018::day1::d1p1("input/1.in".to_string()),
                       aoc2018::day1::d1p2("input/1.in".to_string())),
        _ => println!("unimplemented"),
    };
}
