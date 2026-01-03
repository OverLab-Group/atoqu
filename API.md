
Atoqu HTTP API — v0.6

/search (POST)

Request:

`json
{ "query": "atomic search engine" }
`

Response:

`json
{
  "results": [
    {
      "index": 0,
      "id": "doc1",
      "snippet": "...",
      "score": 0.9,
      "mode": 0
    }
  ]
}
`

/feedback (POST)

Request:

`json
{
  "id": "doc1",
  "mode": 0,
  "score": 1.0,
  "query": "atomic search engine"
}
`

Response:

`json
{ "status": "ok" }
`

/stats (GET)

Returns current mode weights.

