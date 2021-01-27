
use std::collections::HashSet;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

pub fn d1p1(filename: String) -> i32 {
    let mut freq: i32 = 0;

    if let Ok(lines) = read_lines(filename) {
        for line in lines {
            if let Ok(df) = line {
                let f: i32 = df.parse().unwrap();
                freq += f;
            }
        }
    }

    return freq;
}

pub fn d1p2(filename: String) -> i32 {
    let mut dfs: Vec<i32> = Vec::new();
    if let Ok(lines) = read_lines(filename) {
        for line in lines {
            if let Ok(df) = line {
                dfs.push(df.parse().unwrap());
            }
        }
    }

    let mut freq: i32 = 0;
    let mut freqs = HashSet::new();

    loop {
        for df in &dfs {
            freq += df;
            if freqs.contains(&freq) {
                return freq;
            }
            freqs.insert(freq);
        }
    }
}
