package org.uma.ed.datastructures.graph;

import java.util.StringJoiner;

import org.uma.ed.datastructures.dictionary.Dictionary;
import org.uma.ed.datastructures.dictionary.JDKHashDictionary;
import org.uma.ed.datastructures.set.JDKHashSet;
import org.uma.ed.datastructures.set.Set;

/**
 * An implementation of the {@link Graph} interface using an adjacency list representation.
 * <p>
 * The adjacency list is stored in a {@link Dictionary}, where each key is a vertex in the
 * graph, and the corresponding value is a {@link Set} of its adjacent vertices (neighbors).
 * <p>
 * This representation is efficient for sparse graphs (graphs with relatively few edges).
 * Operations like adding a vertex or an edge, and finding the neighbors of a vertex,
 * are typically very fast.
 *
 * @param <V> The type of the vertices in the graph.
 *
 * @author Pepe Gallardo, Data Structures, Grado en Informática. UMA.
 */
public class DictionaryGraph<V> implements Graph<V> {

  /**
   * The core data structure: a dictionary mapping each vertex to its set of neighbors.
   */
  private final Dictionary<V, Set<V>> adjacentsOf;

  /**
   * Constructs an empty {@code DictionaryGraph}.
   */
  public DictionaryGraph() {
    this.adjacentsOf = JDKHashDictionary.empty();
  }

  /**
   * Creates an empty {@code DictionaryGraph}.
   *
   * @param <V> The type for vertices in the graph.
   * @return An empty {@code DictionaryGraph}.
   */
  public static <V> DictionaryGraph<V> empty() {
    return new DictionaryGraph<>();
  }

  /**
   * Creates a new {@code DictionaryGraph} with a given set of vertices and edges.
   *
   * @param <V>      The type for vertices in the graph.
   * @param vertices The initial set of vertices.
   * @param edges    The initial set of edges.
   * @return A new {@code DictionaryGraph} populated with the given data.
   */
  public static <V> DictionaryGraph<V> of(Set<V> vertices, Set<Edge<V>> edges) {
    DictionaryGraph<V> graph = new DictionaryGraph<>();
    for (V vertex : vertices) {
      graph.addVertex(vertex);
    }
    for (Edge<V> edge : edges) {
      graph.addEdge(edge.vertex1(), edge.vertex2());
    }
    return graph;
  }

  /**
   * Creates a new {@code DictionaryGraph} that is a copy of the given graph.
   *
   * @param <V>   The type for vertices in the graph.
   * @param graph The graph to be copied.
   * @return A new {@code DictionaryGraph} with the same vertices and edges.
   */
  public static <V> DictionaryGraph<V> copyOf(Graph<V> graph) { throw new UnsupportedOperationException("Not implemented yet"); }

  @Override
  public boolean isEmpty() { 
    return adjacentsOf.isEmpty();
  }

  @Override
  public void addVertex(V vertex) {
    // Check if vertex already exists in dictionary, do nothing if yes
    if(adjacentsOf.isDefinedAt(vertex)){
      return;
    }
    // Create new set of neighbors for new vertex
    Set<V> s = JDKHashSet.empty(); 
    // Add vertex with its empty set to the dictionary
    adjacentsOf.insert(vertex, s); 
  }

  @Override
  public void addEdge(V vertex1, V vertex2) {
    // Check if the vertex exist
    if(!adjacentsOf.isDefinedAt(vertex1)) {
    addVertex(vertex1); // Create vertex1 with empty set
    }
    if(!adjacentsOf.isDefinedAt(vertex2)) {
        addVertex(vertex2); // Create vertex1 with empty set
    }

    // Get the sets of the neighbors of a node
    Set<V>vertex1neighbors = adjacentsOf.valueOf(vertex1);
    Set<V>vertex2neighbors = adjacentsOf.valueOf(vertex2);

    // Add each vertex on the other set
    vertex1neighbors.insert(vertex2);
    vertex2neighbors.insert(vertex1);
  }

  @Override
  public void deleteEdge(V vertex1, V vertex2) {
    // Check if the vertex exist
    if(!adjacentsOf.isDefinedAt(vertex1) || !adjacentsOf.isDefinedAt(vertex2)) {
      return;
    }
    
    // Get the sets of the neighbors of a node
    Set<V>vertex1neighbors = adjacentsOf.valueOf(vertex1);
    Set<V>vertex2neighbors = adjacentsOf.valueOf(vertex2);

    vertex1neighbors.delete(vertex2);
    vertex2neighbors.delete(vertex1);
  }

  @Override
  public void deleteVertex(V vertex) {
    if(!adjacentsOf.isDefinedAt(vertex)){
        return;
    }

    Set<V> neighbors = adjacentsOf.valueOf(vertex);

    for (V neighbor : neighbors) {
        Set<V> neighborSet = adjacentsOf.valueOf(neighbor);
        if(neighborSet != null) {
            neighborSet.delete(vertex);
        }
    }

    adjacentsOf.delete(vertex);
  }


  @Override
  public Set<V> vertices() { 
    Set<V> vertices = JDKHashSet.empty();

    for(V vertex : adjacentsOf.keys()){
      vertices.insert(vertex);
    }

    return vertices;
  }

  @Override
  public Set<Edge<V>> edges() {
    Set<Edge<V>> edges = JDKHashSet.empty();

    for (V vertex : adjacentsOf.keys()) {
        Set<V> neighbors = adjacentsOf.valueOf(vertex);
        for (V neighbor : neighbors) {
            Edge<V> edge = new Edge<>(vertex, neighbor);
            edges.insert(edge);
        }
    }
    return edges;
  }


  @Override
  public int numberOfVertices() { 
    return adjacentsOf.size();
  }

  @Override
  public int numberOfEdges() { 
    int sum = 0;
    for(V vertex : adjacentsOf.keys()){
      Set<V> neighbors = adjacentsOf.valueOf(vertex);
      int degree = neighbors.size();
      sum += degree;
    }
    return sum / 2;
   }

  @Override
  public Set<V> successors(V vertex) {
    if(!adjacentsOf.isDefinedAt(vertex)){
      return JDKHashSet.empty();
    }

    return adjacentsOf.valueOf(vertex);
  }

  @Override
  public int degree(V vertex) { 
    if(!adjacentsOf.isDefinedAt(vertex)){
      return 0;
    }
    return adjacentsOf.valueOf(vertex).size();
  }

  @Override
  public String toString() {
    String className = getClass().getSimpleName();

    StringJoiner verticesSJ = new StringJoiner(", ", "vertices(", ")");
    for (V vertex : vertices()) {
      verticesSJ.add(vertex.toString());
    }

    StringJoiner edgesSJ = new StringJoiner(", ", "edges(", ")");
    for (Edge<V> edge : edges()) {
      edgesSJ.add(edge.toString());
    }

    StringJoiner sj = new StringJoiner(", ", className + "(", ")");
    sj.add(verticesSJ.toString());
    sj.add(edgesSJ.toString());
    return sj.toString();
  }
}