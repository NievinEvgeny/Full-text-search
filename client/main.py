import fts
import argparse
import os

parser = argparse.ArgumentParser(description='Search engine')

parser.add_argument("--i", type=str, default="index", help="Name of folder containing indexed data")

args = parser.parse_args()

index = args.i.encode("utf-8")

query = input("Enter your search query:\n").encode("utf-8")
os.system('clear')

index_accessor = fts.TextIndexAccessor(index)
searcher = fts.Searcher(index_accessor._obj)

search_info = searcher.score_calc(query)
searcher.print_scores(search_info)




