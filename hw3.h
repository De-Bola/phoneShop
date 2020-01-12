//
// Created by admin on 1/10/2020.
//

#ifndef HW_3_MINE_HW3_H
#define HW_3_MINE_HW3_H

#define LENGTH_OF_CHAR 32
#define LOGFILE "Log.txt"

typedef struct {
    int productCode;
    char *mark;
    char *model;
    int ram;
    float screenSize;
    char *operatingSystem;
} Product;

typedef struct {
    int quote_id;
    int productCode;
    char *retailer;
    int price;
    char *status;
} Quote;

typedef struct quoteNode {
    Quote quote;
    struct quoteNode *next;
    struct quoteNode *previous;
    int index;
} quoteNode;

typedef struct productNode {
    Product product;
    struct quoteNode *quote;
    struct productNode *next;
    struct productNode *previous;
    int index;
} productNode;

productNode *readProducts(productNode *products);

quoteNode *readQuotes(quoteNode *quotes);

productNode *getProductData(FILE *productFile, Product productData);

quoteNode *getQuoteData(FILE *quoteFile, Quote quoteData);

productNode *makeProductNode(Product productData);

quoteNode *makeQuoteNode(Quote quoteData);

productNode *appendProductNode(productNode *head, Product productData);

void appendQuoteNode(quoteNode *head, Quote quoteData);

void printProductsListAsc(productNode *products);

void printQuotesListAsc(quoteNode *quotes);

quoteNode *appendSingleQuoteNode(quoteNode *head, Quote quoteData);

quoteNode *sortQuotesById(quoteNode *head, quoteNode *tail);

quoteNode *changeQuoteNodeHead(quoteNode *head, quoteNode *nextHead);

productNode *changeProductNodeHead(productNode *head, productNode *nextHead);

productNode *addQuotesToProduct(productNode *products, quoteNode *quotes);

void constructMainList(productNode *fullProducts, quoteNode *fullQuotes);

bool searchByProductCode(productNode *products, int productCode);

bool searchByQuoteId(quoteNode *products, int quote_id);

int checkUserChoice(char *answer);

void addNewQuote(quoteNode *quotes, int productCode);

void addNewProduct(productNode *products, int productCode);

Product allocProductData(char *mark, char *model, char *operatingSystem, Product productData);

Quote allocQuoteData(char *retailer, char *status, Quote quoteData);

void deleteSingleQuote(quoteNode *products, int quote_id, int productCode);

bool checkForQuotes(quoteNode *rTag, int code);

productNode *deleteSingleProduct(productNode *products, quoteNode *quotes, int productCode);

quoteNode *deleteQuotesInBulk(quoteNode *quotes, int productCode);

void writeChangesToFile(productNode *pTag, quoteNode *qTag);

void printProductsToFile(productNode *products, FILE *productFile);

void printQuotesToFile(quoteNode *quotes, FILE *quoteFile);

quoteNode *editQuote(quoteNode *quotes, int productCode);

productNode *editProduct(productNode *products, int productCode);

Product getProductDataFromUser(int productCode);

Quote getQuoteById(quoteNode *quotes, int quote_id);

Product getProductByProductCode(productNode *products, int productCode);

void resetQuoteIndexes(quoteNode *quotes);

void resetProductIndexes(productNode *products);

void showUpdatedQuotes(quoteNode *quoteList);

void showUpdatedProducts(productNode *productList);

void bubbleSortQuoteList(quoteNode *head);

void swap(quoteNode *quote, quoteNode *nextQuote);

void showProduct(productNode *products, int productCode);

quoteNode *getAvailablePhones(quoteNode *quotes);

productNode *getPhonesByModel(productNode *products, char *userInput);

productNode *getPhonesByMaker(productNode *products, char *userInput);

productNode *appendSingleProductNode(productNode *head, productNode *tail);

void freeProductList(productNode *product);

void freeQuoteList(quoteNode *quote);

#endif //HW_3_MINE_HW3_H