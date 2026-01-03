Modes
=====

NormalMode
----------

Browser-style, classic search combining literal and vector signals.

LiteralMode
-----------

Keyword-based search over document text.

VectorMode
----------

Embedding-based similarity search using VectorStore.

HybridMode
----------

Combines LiteralMode and VectorMode scores with configurable weights.

BM25Mode
--------

BM25-like term-based ranking for text-heavy documents.

RecencyMode
-----------

Boosts documents with newer timestamps.

TagBoostMode
------------

Boosts documents whose tags match query tokens.
