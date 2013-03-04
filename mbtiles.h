typedef struct {
    int zoom_level;
    int column;
    int row;
    const char *data;
    int data_size;
} MBTile;

typedef struct {
    char* name;
    char* type;
    char* version;
    char* description;
    char* format;
    char* bounds;
} MBTilesMetadata;

/*
 *   Opaque C-style object.
 */
typedef struct MBTilesIterator MBTilesIterator;

#ifdef __cplusplus
extern "C" {
#endif
  
/*
 *   Allocate new iterator, initialize it for reading `path` and set `self_ptr` to point to it.
 *   Return zero on success, negative value on failure.
 */
int mbtiles_iterator_new(MBTilesIterator **self_ptr, MBTilesMetadata *metadata, const char *path);

/*
 *  Clear all resources and deallocate the iterator.
 */
void mbtiles_iterator_free(MBTilesIterator *self);

/*
 *   Return true value iff the iterator has been exhausted.
 */
int mbtiles_iterator_finished(const MBTilesIterator *self);

/*
 *   Given that the iterator has not been exhausted, move to the next tile
 *   and fill `tile` with information about it.
 *   Return 0 on success, negative value on error.
 */
int mbtiles_iterator_get(MBTilesIterator *self, MBTile *tile);

#ifdef __cplusplus
}
#endif
