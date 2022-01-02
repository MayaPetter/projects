package il.co.ilrd.selector;

import java.io.IOException;
import java.io.Serializable;

public interface CRUD<K extends Serializable, D extends Serializable> extends AutoCloseable{

    public K create(D data) throws IOException;

    public D read(K key) throws IOException;

    public void update(K key, D newData) throws IOException;

    public void delete(K key) throws IOException;
}