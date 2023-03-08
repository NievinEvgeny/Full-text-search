import ctypes

fts_lib = ctypes.cdll.LoadLibrary('libfts-c.so')

class TermInfo(ctypes.Structure):
    _fields_ = [
        ('doc_id', ctypes.c_int),
        ('term_freq', ctypes.c_int)
    ]

class DocScore(ctypes.Structure):
    _fields_ = [
        ('doc_id', ctypes.c_int),
       ('score', ctypes.c_double)
   ]

class SearchInfo(ctypes.Structure):
    _fields_ = [
        ('docs_scores', ctypes.c_void_p),
        ('num_of_terms', ctypes.c_size_t),
        ('num_of_docs', ctypes.c_size_t)
    ]

text_index_accessor_new = fts_lib.text_index_accessor_new
text_index_accessor_new.argtypes = [ctypes.c_char_p]
text_index_accessor_new.restype = ctypes.c_void_p

index_accessor_delete = fts_lib.index_accessor_delete
index_accessor_delete.argtypes = [ctypes.c_void_p]

index_accessor_total_docs = fts_lib.index_accessor_total_docs
index_accessor_total_docs.argtypes = [ctypes.c_void_p]
index_accessor_total_docs.restype = ctypes.c_size_t

index_accessor_load_document = fts_lib.index_accessor_load_document
index_accessor_load_document.argtypes = [ctypes.c_void_p, ctypes.c_int]
index_accessor_load_document.restype = ctypes.c_char_p

index_accessor_get_term_infos = fts_lib.index_accessor_get_term_infos
index_accessor_get_term_infos.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
index_accessor_get_term_infos.restype = ctypes.POINTER(TermInfo)

class IndexAccessor:
    def __init__(self, obj):
        self._obj = obj

    def __del__(self):
        index_accessor_delete(self._obj)

    def total_docs(self):
        return index_accessor_total_docs(self._obj)

    def load_document(self, doc_id: ctypes.c_int):
        return index_accessor_load_document(self._obj, doc_id)

    def get_term_infos(self, term: ctypes.c_char_p):
        return index_accessor_get_term_infos(self._obj, term)

class TextIndexAccessor(IndexAccessor):
    def __init__(self, dir_name: ctypes.c_char_p):
        obj = text_index_accessor_new(dir_name)
        super(TextIndexAccessor, self).__init__(obj)

searcher_new = fts_lib.searcher_new
searcher_new.argtypes = [ctypes.c_void_p]
searcher_new.restype = ctypes.c_void_p

searcher_delete = fts_lib.searcher_delete
searcher_delete.argtypes = [ctypes.c_void_p]

searcher_print_scores = fts_lib.searcher_print_scores
searcher_print_scores.argtypes = [ctypes.c_void_p, ctypes.POINTER(SearchInfo)]

searcher_score_calc = fts_lib.searcher_score_calc
searcher_score_calc.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
searcher_score_calc.restype = ctypes.POINTER(SearchInfo)

searcher_set_accessor = fts_lib.searcher_set_accessor
searcher_set_accessor.argtypes = [ctypes.c_void_p, ctypes.c_void_p]

class Searcher:
    def __init__(self, accessor: TextIndexAccessor):
        self._obj = searcher_new(accessor)
        
    def __del__(self):
        searcher_delete(self._obj)

    def set_accessor(self, new_accessor: TextIndexAccessor):
        searcher_set_accessor(self._obj, new_accessor)

    def score_calc(self, query: ctypes.c_char_p):
        return searcher_score_calc(self._obj, query)
    
    def print_scores(self, info: SearchInfo):
        searcher_print_scores(self._obj, info)
