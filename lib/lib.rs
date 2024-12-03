use std::io;
use std::io::BufRead;
use std::fs;

pub fn file_lines( path : &str ) -> io::Result< impl Iterator<Item=String>> {
    let file = fs::File::open( path )?;
    let reader = io::BufReader::new( file );
    Ok(reader.lines().map(|maybeline| maybeline.unwrap_or( "".to_string())))
}

