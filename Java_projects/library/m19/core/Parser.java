package m19.core;

import java.util.Collections;
import java.util.List;
import java.util.ArrayList;

import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.FileNotFoundException;

import m19.core.exception.BadEntrySpecificationException;
import m19.core.exception.BadEntryUserException;

public class Parser {
	private Library _library;

	public Parser(Library lib) {
		_library = lib;
	}

	protected void parse(String filename) throws BadEntrySpecificationException, IOException, BadEntryUserException {
		String line;
		String[] parsed;
		try(BufferedReader reader = new BufferedReader(new FileReader(filename))) {
			if( reader == null )
				return;
		
			while( (line = reader.readLine()) != null ) {
				parsed = line.split(":");
				switch(parsed[0]) {
					case "DVD":
						if( parsed.length != 7 )
							throw new BadEntrySpecificationException("Invalid Work Specification");
						parseDvd(parsed);
						break;
					case "BOOK":
						if( parsed.length != 7 )
							throw new BadEntrySpecificationException("Invalid Work Specification");
						parseBook(parsed);
						break;

					case "USER":
						if( parsed.length != 3 )
							throw new BadEntrySpecificationException("Invalid User Specification");
						parseUser(parsed);
						break;

					default:
						throw new BadEntrySpecificationException("Invalid Specification");
				}
			}
		} 
	}


	private void parseDvd(String[] props) {
		_library.addDvd(props[1], Integer.parseInt(props[3]), Integer.parseInt(props[6]), props[4], props[2], props[5]);
	}

	private void parseBook(String[] props) {
		_library.addBook(props[1], Integer.parseInt(props[3]), Integer.parseInt(props[6]), props[4], props[2], props[5]);
	}

	private void parseUser(String[] props) throws BadEntryUserException {
		_library.registerUser(props[1], props[2]);
	}
}