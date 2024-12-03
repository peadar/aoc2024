use std::{env,io};
use std::collections::HashMap;


struct Node {
    children : HashMap<char, Node>,
    is_word : bool,
}


struct Trie {
    root : Node,
}

impl Node {
    fn new() -> Self {
        Node {
            children: HashMap::new(),
            is_word: false,
        }
    }
}

impl Trie {
    fn add_word( &mut self, text : &str) {
        /*
        let mut node = &self.root;
        for c in text.chars() {
            let mut entry = &node.children.entry(c).or_insert_with(|| Node::new());
            node = entry;
        }
        */
    }
}


fn parse_file( path : &str ) -> io::Result<Vec<Vec<char>>> {
    Ok(aoc::file_lines( path )?.map(|line| line.chars().collect()).collect())
}

fn main() {
    let args:Vec<String> = env::args().collect();
    let data = parse_file(&args[1]).expect("the data is invalid");


}
