package il.co.ilrd.selector;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.io.RandomAccessFile;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;


public class FileModifier implements CRUD<Integer, String>{
	private File file;
	private LineNumberReader reader;
	private RandomAccessFile writer;

	public FileModifier(String pathToFile) throws IOException{
		file = new File(pathToFile); 
		if(!file.isFile()) {
			throw new IllegalArgumentException("not a file");
		}
		writer = new RandomAccessFile(file, "rw");
		reader = new LineNumberReader(new FileReader(file));
	}

	@Override//append
	public Integer create(String data) throws IOException{
			writer.seek(writer.length());
			writer.write((data + System.lineSeparator()).getBytes());
			addvanceToEOF();
		return reader.getLineNumber();
	}

	@Override
	public String read(Integer lineNum) throws IOException{
		reader.setLineNumber(lineNum.intValue());
		return reader.readLine();
	}

	@Override
	public void update(Integer lineNum, String newData) throws IOException {
		Objects.requireNonNull(lineNum);
		List<String> fileContent = new ArrayList<>(Files.readAllLines(file.toPath(), StandardCharsets.UTF_8));
		try {
			fileContent.set(lineNum, newData);
		}catch (Exception e) {
			throw new IOException(e.getCause());
		}
		Files.write(file.toPath(), fileContent, StandardCharsets.UTF_8);
	}

	@Override
	public void delete(Integer lineNum) throws IOException {
		Objects.requireNonNull(lineNum);
		List<String> fileContent = new ArrayList<>(Files.readAllLines(file.toPath(), StandardCharsets.UTF_8));
		try {
			fileContent.remove(lineNum.intValue());
		}catch (Exception e) {
			throw new IOException(e.getCause());
		}
		Files.write(file.toPath(), fileContent, StandardCharsets.UTF_8);
	}

	@Override
	public void close() throws Exception {
		writer.close();
		reader.close();
	}

	private void addvanceToEOF() throws IOException {
		while (null != reader.readLine()) {
			/*read until reaching end of file */
		}
	}
}