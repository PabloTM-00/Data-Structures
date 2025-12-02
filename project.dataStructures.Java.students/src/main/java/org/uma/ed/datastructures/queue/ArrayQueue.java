package org.uma.ed.datastructures.queue;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * An implementation of the {@link Queue} interface using a dynamic circular array.
 * <p>
 * This class provides a FIFO (First-In-First-Out) queue. The capacity of the
 * underlying array is automatically increased by doubling its size when it becomes full.
 *
 * @param <T> The type of elements held in this queue.
 *
 * @author Pepe Gallardo, Data Structures, Grado en Informática. UMA.
 */
public class ArrayQueue<T> extends AbstractQueue<T> implements Queue<T> {

  /**
   * Default initial capacity for the queue if none is provided.
   */
  private static final int DEFAULT_INITIAL_CAPACITY = 16;

  /**
   * Array buffer to store the elements of the queue.
   */
  private T[] elements;

  /**
   * Index of the first element in the queue (the head).
   */
  private int first;

  /**
   * Index of the last element in the queue (the tail).
   */
  private int last;

  /**
   * The number of elements in the queue.
   */
  private int size;

  /*
   * INVARIANT (Circular Array Logic):
   *  - The `size` field holds the number of elements currently in the queue.
   *  - If the queue is empty (`size == 0`), the `first` and `last` indices are not
   *    meaningfully pointing to elements. By convention, `first` is 0 and `last` is `elements.length - 1`.
   *  - If the queue is not empty:
   *      - `elements[first]` stores the element at the front of the queue.
   *      - `elements[last]` stores the element at the rear of the queue.
   *      - The elements are located at indices `first`, `(first + 1) % length`, ..., `last`.
   */

  /**
   * Constructs an empty {@code ArrayQueue} with a specified initial capacity.
   * <p> Time complexity: O(1)
   *
   * @param initialCapacity the initial capacity of the underlying array.
   * @throws IllegalArgumentException if the specified initial capacity is not positive.
   */
  @SuppressWarnings("unchecked")
  public ArrayQueue(int initialCapacity) {
    if (initialCapacity <= 0) {
      throw new IllegalArgumentException("Initial capacity must be greater than 0");
    }
    this.elements = (T[]) new Object[initialCapacity];
    this.size = 0;
    this.first = 0;
    this.last = initialCapacity - 1; // So that the first `advance(last)` results in 0
  }

  /**
   * Constructs an empty {@code ArrayQueue} with a default initial capacity.
   * <p> Time complexity: O(1)
   */
  public ArrayQueue() {
    this(DEFAULT_INITIAL_CAPACITY);
  }

  /**
   * Creates an empty {@code ArrayQueue} with a default initial capacity.
   * <p> Time complexity: O(1)
   *
   * @param <T> the type of elements in the queue.
   * @return an empty {@code ArrayQueue}.
   */
  public static <T> ArrayQueue<T> empty() {
    return new ArrayQueue<>();
  }

  /**
   * Creates an empty {@code ArrayQueue} with a specified initial capacity.
   * <p> Time complexity: O(1)
   *
   * @param <T>             the type of elements in the queue.
   * @param initialCapacity the initial capacity for the queue.
   * @return an empty {@code ArrayQueue} with the given capacity.
   * @throws IllegalArgumentException if the initial capacity is not positive.
   */
  public static <T> ArrayQueue<T> withCapacity(int initialCapacity) {
    return new ArrayQueue<>(initialCapacity);
  }

  /**
   * Creates a new {@code ArrayQueue} containing the given elements.
   * <p> Time complexity: O(n), where n is the number of elements.
   *
   * @param <T>      the type of elements.
   * @param elements the elements to be enqueued.
   * @return a new {@code ArrayQueue} with the given elements.
   */
  @SafeVarargs
  public static <T> ArrayQueue<T> of(T... elements) {
    ArrayQueue<T> queue = new ArrayQueue<>(elements.length > 0 ? elements.length : DEFAULT_INITIAL_CAPACITY);
    for (T element : elements) {
      queue.enqueue(element);
    }
    return queue;
  }

  /**
   * Creates a new {@code ArrayQueue} from an iterable collection.
   * <p> Time complexity: O(n), where n is the number of elements.
   *
   * @param <T>      the type of elements.
   * @param iterable the iterable containing elements to be enqueued.
   * @return a new {@code ArrayQueue} with the given elements.
   */
  public static <T> ArrayQueue<T> from(Iterable<T> iterable) {
    ArrayQueue<T> queue = new ArrayQueue<>();
    for (T element : iterable) {
      queue.enqueue(element);
    }
    return queue;
  }

  /**
   * Creates a new {@code ArrayQueue} containing the same elements as the given queue.
   * <p>
   * This is an efficient O(n) operation that copies the internal state directly.
   * <p> Time complexity: O(n)
   *
   * @param <T>  the type of elements.
   * @param that the {@code ArrayQueue} to be copied.
   * @return a new {@code ArrayQueue} with the same elements and order.
   */
  public static <T> ArrayQueue<T> copyOf(ArrayQueue<T> that) {
    // Creamos una cola nueva con la misma capacidad
    ArrayQueue<T> copy = new ArrayQueue<>(that.elements.length);

    // Copiamos los elementos "desenrollando" el círculo
    // Empezamos a leer desde el 'first' de la original
    int current = that.first;
    
    // Escribimos en la copia desde 0 en adelante
    for (int i = 0; i < that.size; i++) {
        copy.elements[i] = that.elements[current]; // Copia directa
        current = (current + 1) % that.elements.length; // Avanza circular en la original
    }

    // Ajustamos los índices de la copia (ahora está "desenrollada")
    copy.first = 0;
    copy.size = that.size;
    copy.last = that.size - 1; // El último está justo al final de los datos
    // Si size es 0, last debe ser capacity - 1 (el caso del constructor)
    if (copy.size == 0) {
        copy.last = copy.elements.length - 1;
    }
    return copy;
  }
  

  /**
   * Creates a new {@code ArrayQueue} containing the same elements as the given queue.
   * <p>
   * Note: This operation temporarily modifies the source queue by dequeueing all its
   * elements, but restores it to its original state before returning. The efficiency of this
   * method depends heavily on the implementation of the source queue.
   * <p> Time complexity: O(n * T_first + n * T_enqueue + n * T_dequeue),
   * where T_... is the complexity of the source queue's operations.
   *
   * @param <T>  the type of elements.
   * @param that the generic {@code Queue} to be copied.
   * @return a new {@code ArrayQueue} with the same elements and order.
   */
  public static <T> ArrayQueue<T> copyOf(Queue<T> that) {
	  // 1. Creamos la copia (con tamaño suficiente)
    int size = that.size(); // Guardamos el tamaño original
    // Usamos Math.max(1, size) para evitar error si está vacía
    ArrayQueue<T> copy = new ArrayQueue<>(Math.max(1, size)); 

    // 2. Transvase (Esto vacía 'that')
    // Sacamos de 'that' y metemos en 'copy'
    for (int i = 0; i < size; i++) {
        T element = that.first(); // Miramos
        that.dequeue();           // Sacamos
        copy.enqueue(element);    // Guardamos en copia
    }

    // 3. Restauración (Esto repara 'that')
    // Ahora 'that' está vacía y 'copy' llena. 
    // Recorremos 'copy' (sin vaciarla) para volver a llenar 'that'.
    
    // Como 'copy' es ArrayQueue, podemos iterar sobre ella sin romperla si usamos el iterador
    // O podemos hacer el truco de rotarla:
    for (int i = 0; i < size; i++) {
        T element = copy.first();
        copy.dequeue();
        copy.enqueue(element); // La volvemos a meter en copy (rotación)
        that.enqueue(element); // La devolvemos a su sitio original
    }
    return copy;
  }
  /**
   * {@inheritDoc}
   * <p> Time complexity: O(1)
   */
  @Override
  public boolean isEmpty() {
	  return size == 0;
  }

  /**
   * {@inheritDoc}
   * <p> Time complexity: O(1)
   */
  @Override
  public int size() { 
    return size; 
  }

  /**
   * Advances an index, wrapping around the array if necessary.
   * @param index the current index.
   * @return the next index in the circular array.
   */
  private int advance(int index) {
    return (index + 1) % elements.length;
  }

  /**
   * Ensures capacity is sufficient for one more element. If the array is full, it is
   * resized to double its capacity, and the elements are linearized.
   */
  private void ensureCapacity() {
    if (size == elements.length) {
      @SuppressWarnings("unchecked")
      T[] newElements = (T[]) new Object[2 * elements.length];

      // Copy elements to the new array, linearizing them in the process.
      int current = this.first;
      for (int i = 0; i < size; i++) {
        newElements[i] = elements[current];
        current = advance(current);
      }

      this.elements = newElements;
      this.first = 0;
      this.last = size - 1;
    }
  }

  /**
   * {@inheritDoc}
   * <p> Time complexity: Amortized O(1). A single enqueue may take O(n) if the
   * underlying array needs to be resized.
   */
  @Override
public void enqueue(T element) { 
    ensureCapacity(); // Si está llena, duplica y realinea
    last = advance(last); // Avanza circularmente: (last + 1) % length
    elements[last] = element;
    size++;
}

  /**
   * {@inheritDoc}
   * <p> Time complexity: O(1)
   */
  @Override
  public T first() {
    return elements[0];

  }

  /**
   * {@inheritDoc}
   * <p> Time complexity: O(1)
   */
  @Override
public void dequeue() {      
    if(isEmpty()) {
        throw new EmptyQueueException("dequeue on empty queue");
    }
    elements[first] = null; // Evitar memory leaks
    first = advance(first); // Avanza circularmente
    size--;
}

  /**
   * {@inheritDoc}
   * <p> Time complexity: O(n)
   */
  @Override
  public void clear() {
    // Auxiliary pointer (circular clearing)
    int current = first;
    for(int i = 0; i < size; i++){
      elements[current] = null;
      current = advance(current);
    }
    first = 0;
    last =  elements.length - 1 ;
    size = 0;
  }


  /**
   * Provides an iterable that traverses the elements from the front to the rear of the queue.
   *
   * @return an {@code Iterable} for the elements of the queue.
   */
  @Override
  protected Iterable<T> elements() {
    return () -> new Iterator<>() {
      private int current = first;
      private int count = 0;

      @Override
      public boolean hasNext() {
        return count < size;
      }

      @Override
      public T next() {
        if (!hasNext()) {
          throw new NoSuchElementException();
        }
        T element = elements[current];
        current = advance(current);
        count++;
        return element;
      }
    };
  }
}