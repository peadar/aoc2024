use std::env;
use std::collections::HashMap;
use ::aoc;
use ::std::io;

struct Data {
    cols : [ Vec<i32>; 2 ],
}

impl Clone for Data {
    fn clone(&self) -> Self {
        Data {
            cols: self.cols.clone(),
        }
    }
}

fn parse_file( path : &str ) -> io::Result<Data> {

    let mut res = Data {
        cols : [ Vec::new(), Vec::new() ],
    };

    for line in aoc::file_lines( path )? {
        let mut col = 0;
        for tok in line.split_whitespace() {
            res.cols[col].push( tok.parse::<i32>().expect("invalid integer") );
            col += 1;
        }
    }
    Ok(res)
}

fn part1( data : &mut Data ) -> i32 {
    for vec in data.cols.iter_mut() {
        vec.sort();
    }
    data.cols[0].iter().zip(data.cols[1].iter()).map(|(l,r)| (l-r).abs()).sum()
}

fn part2( data : Data ) -> i32 {
    let rhs_counts = data.cols[0].iter().fold(HashMap::<i32,i32>::new(), |mut counts, value| {
        *counts.entry(*value).or_default() += 1;
        counts
    });
    data.cols[1].iter().map( |datum| datum * rhs_counts.get(datum).unwrap_or(&0)).sum()
}

fn main() -> io::Result<()> {
    let args:Vec<String> = env::args().collect();
    let mut data = parse_file(&args[1]).expect("the data is invalid");
    println!("part1: {}", part1(&mut data));
    println!("part2: {}", part2(data));
    Ok(())
}

