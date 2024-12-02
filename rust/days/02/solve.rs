use std::io;
use std::env;
use ::aoc;

fn parse_file( path : &str ) -> io::Result<Vec<Vec<i32>>> {
    let mut res = Vec::new();
    for line in aoc::file_lines( path )? {
        let mut row : Vec<i32> = Vec::new();
        for tok in line.split_whitespace() {
            row.push( tok.parse::<i32>().expect("invalid integer") );
        }
        res.push( row );
     }
    Ok(res)
}

fn first_order_difference<'a, I>( mut iter : I ) -> impl Iterator<Item = i32> + 'a
where
    I : Iterator<Item = i32> + 'a
{
    let mut prev = iter.next().expect("diff needs at least one item in its input");
    iter.map(move |i| {
        let next = i - prev; 
        prev = i;
        next
    })
}

fn accept<I>( iter : I ) -> bool
where
    I : Iterator<Item = i32> + Clone
{
    first_order_difference(iter.clone()).all(|v| v >= 1 && v <= 3) ||
    first_order_difference(iter.clone()).all(|v| v <= -1 && v >= -3)
}

fn remove_item<'a, I>( iter : I, remove_idx : usize ) -> impl Iterator<Item = i32> + 'a + Clone
where
    I : Iterator<Item = i32> + 'a  + Clone
{
    iter.enumerate().filter( move |(idx, _)| *idx != remove_idx ).map( |(_, value)| value)
}

fn part1( cols : &Vec<Vec<i32>> ) -> usize {
    cols.iter().filter( |&row|  accept(row.iter().map(|v| *v))).count()
}

fn part2( cols : &Vec<Vec<i32>> ) -> usize {
    cols.iter().filter( |&row| (0..row.len()+1).any( |ignore| accept( remove_item( row.iter().map(|v| *v), ignore ) ) )).count()
}

fn main() -> io::Result<()> {
    let args:Vec<String> = env::args().collect();
    let data = parse_file(&args[1]).expect("the data is invalid");
    println!("part1: {}", part1(&data));
    println!("part2: {}", part2(&data));
    Ok(())
}
