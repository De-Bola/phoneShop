#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "hw3.h"

int main(void) {
    productNode *products = NULL;
    quoteNode *quotes = NULL;
    productNode *productList = NULL;
    quoteNode *quoteList = NULL;
    FILE *log;
    int input = 0, quote_id = 0, productCode = 0;
    int isYes = 0, i = 0;
    bool isExists = false, isQuoteExists = false, isProductExists = false, hasQuotes = false;
    char answer[LENGTH_OF_CHAR];
    char userInput[LENGTH_OF_CHAR];
    char *maker;
    char *model;
    quoteNode *availablePhones = NULL;
    productNode *searchResult = NULL;
    Quote quoteFromFile;
    Product productFromFile;
    time_t currentTime = time(0);

    log = fopen(LOGFILE, "a+");
    if (log == NULL) {
        printf("Unable to open the file\n");
        exit(1300);
    }

    products = readProducts(products);
    quotes = readQuotes(quotes);

    productList = readProducts(productList);
    quoteList = readQuotes(quoteList);
//    printProductsListAsc (products);
//    printQuotesListAsc (quotes);

    while (quotes->previous != NULL) {
        quotes = quotes->previous;
    }//go to head of quotes list
    while (products->previous != NULL) {
        products = products->previous;
    }//go to head of products list

    //printListDesc (products);

    bubbleSortQuoteList(quotes);
    //printQuotesListAsc(quotes);
    constructMainList(products, quotes);

    do {
        printf("Press '1' to see all entries\n");
        printf("Press '2' to edit entries\n");
        printf("Press '3' to search through entries\n");
        printf("Press '4' to delete an entry\n");
        printf("Press '5' to add an entry\n");
        printf("And press '0' to check-out\n**\tPlease Note: All other entries won't be recognized!'\t**\n");
        scanf("%d", &input);
        fprintf(log, "%d\n", input);
        fprintf(log, "visited main menu at %s\n", ctime(&currentTime));
        switch (input) {
            case 1://prints all items
                fprintf(log, "All items viewed\n");
                int displayInput = 0;
                do {
                    printf("Press '1' to print quote list\n");
                    printf("Press '2' to print product list\n");
                    printf("Press '3' to print joined list\n");
                    printf("And press '0' to go back\n**\t"
                           "Please Note: All other entries won't be recognized!'\t**\n");
                    scanf("%d", &displayInput);
                    fprintf(log, "%d\n", displayInput);
                    fprintf(log, "%s", ctime(&currentTime));
                    switch (displayInput) {
                        case 1://show quote list
                            printQuotesListAsc(quoteList);
                            fprintf(log, "viewed quote list at %s\n", ctime(&currentTime));
                            break;
                        case 2://show product list
                            printProductsListAsc(productList);
                            fprintf(log, "viewed product list at %s\n", ctime(&currentTime));
                            break;
                        case 3://show joined list
                            printProductsListAsc(products);
                            fprintf(log, "viewed joined list at %s\n", ctime(&currentTime));
                            break;
                        default:
                            printf("Please try again!!\n");
                            fprintf(log, "Invalid entry\n");
                    }
                } while (displayInput != 0);
                break;
            case 2://edits available items
                fprintf(log, "user chose to edit entries\n");
                int editInput = 0;
                do {
                    printf("Press '1' to edit a quote\n");
                    printf("Press '2' to edit a product\n");
                    printf("And press '0' to go back\n**\t"
                           "Please Note: All other entries won't be recognized!'\t**\n");
                    scanf("%d", &editInput);
                    fprintf(log, "%d\n", editInput);
                    fprintf(log, "navigated to edit menu at %s\n", ctime(&currentTime));
                    switch (editInput) {
                        case 1:
                            fprintf(log, "user chose to edit a quote\n");
                            printf("Please enter a productCode:\n");
                            scanf("%d", &productCode);
                            isProductExists = searchByProductCode(productList, productCode);
                            if (!isProductExists) {
                                printf("This productCode doesn't exist, please try another productCode\n");
                                fprintf(log, "discarded quote list edit at %s\n", ctime(&currentTime));
                                break;
                            }

                            showProduct(products, productCode);

                            quoteList = editQuote(quoteList, productCode);
                            showUpdatedQuotes(quoteList);
                            fprintf(log, "edited quote list at %s\n", ctime(&currentTime));
                            break;
                        case 2:
                            fprintf(log, "user chose to edit a product\n");
                            do {
                                do {
                                    printf("Please enter a productCode:\n");
                                    scanf("%d", &productCode);
                                    isProductExists = searchByProductCode(productList, productCode);
                                    if (!isProductExists) {
                                        printf("This productCode doesn't exist, please try another productCode\n");
                                    }
                                } while (!isProductExists);
                                productFromFile = getProductByProductCode(productList, productCode);

                                printf("Is this the product you wish to change?[Y/N]\n");
                                printf("%d\t%s\t%s\t%d\t%.2f\t%s\n", productFromFile.productCode, productFromFile.mark,
                                       productFromFile.model, productFromFile.ram, productFromFile.screenSize,
                                       productFromFile.operatingSystem);
                                scanf("%s", answer);
                                isYes = checkUserChoice(answer);
                                if (isYes < 0) {
                                    printf("Invalid entry, please try again!!\n");
                                }
                            } while (isYes < 1);
                            productList = editProduct(productList, productCode);
                            showUpdatedProducts(productList);
                            fprintf(log, "edited product list at %s\n", ctime(&currentTime));
                            break;
                        default:
                            printf("Please try again!!\n");
                            fprintf(log, "Invalid entry\n");
                    }
                } while (editInput != 0);

            case 3://search by product_code
                fprintf(log, "Search through inventory\n");
                int searchInput = 0;
                do {
                    printf("Press '1' to display available phones\n");
                    printf("Press '2' to search by maker\n");
                    printf("Press '3' to search by model\n");
                    printf("And press '0' to go back\n**\t"
                           "Please Note: All other entries won't be recognized!'\t**\n");
                    scanf("%d", &searchInput);
                    fprintf(log, "%d\n", searchInput);
                    fprintf(log, "user navigated to search menu at %s\n", ctime(&currentTime));
                    switch (searchInput) {
                        case 1://search based on availability
                            availablePhones = getAvailablePhones(quoteList);
                            printQuotesListAsc(availablePhones);
                            fprintf(log, "user viewed available phones at %s\n", ctime(&currentTime));
                            break;
                        case 2://to search by maker
                            printf("Please enter a phone maker:\n");
                            scanf("%s", userInput);
                            for (i = 0; i < strlen(userInput); i++) {
                                userInput[i] = tolower(userInput[i]);
                            }
                            maker = calloc(strlen(userInput) + 1, sizeof(char));
                            if (!maker) {
                                free(maker);
                                exit(10);
                            }
                            strcpy(maker, userInput);
                            searchResult = getPhonesByMaker(products, maker);
                            bubbleSortQuoteList(quotes);
                            constructMainList(searchResult, quotes);
                            printProductsListAsc(searchResult);
                            free(maker);
                            fprintf(log, "searched by phone maker at %s\n", ctime(&currentTime));
                            break;
                        case 3://to search by model
                            printf("Please enter a phone model:\n");
                            scanf("%s", userInput);
                            for (i = 0; i < strlen(userInput); i++) {
                                userInput[i] = tolower(userInput[i]);
                            }
                            model = calloc(strlen(userInput) + 1, sizeof(char));
                            if (!model) {
                                free(model);
                                exit(10);
                            }
                            strcpy(model, userInput);
                            searchResult = getPhonesByModel(products, model);
                            bubbleSortQuoteList(quotes);
                            constructMainList(searchResult, quotes);
                            printProductsListAsc(searchResult);
                            free(model);
                            fprintf(log, "searched by phone model at %s\n", ctime(&currentTime));
                            break;
                        default:
                            printf("Please try again!!\n");
                            fprintf(log, "Invalid entry\n");
                    }
                } while (searchInput != 0);
                break;
            case 4://delete an entry
                fprintf(log, "user chose to delete an item\n");
                int deleteInput;
                do {
                    printf("Press '1' to remove a quote\n");
                    printf("Press '2' to remove a product\n");
                    printf("And press '0' to check-out\n**\t"
                           "Please Note: All other entries won't be recognized!'\t**\n");
                    scanf("%d", &deleteInput);
                    fprintf(log, "%d\n", input);
                    fprintf(log, "user navigated to delete menu at %s\n", ctime(&currentTime));
                    switch (deleteInput) {
                        case 1://remove one quote
                            fprintf(log, "user chose to delete a quote\n");
                            do {
                                printf("Please enter a productCode:\n");
                                scanf("%d", &productCode);
                                isProductExists = searchByProductCode(products, productCode);
                                if (!isProductExists) {
                                    printf("This productCode doesn't exist, please try another productCode\n");
                                }
                            } while (!isProductExists);

                            do {
                                printf("Please enter a quote_id:\n");
                                scanf("%d", &quote_id);
                                quoteFromFile = getQuoteById(quoteList, quote_id);
                                isQuoteExists = productCode == quoteFromFile.productCode;
                                if (!isQuoteExists) {
                                    printf("This quote_id doesn't exist, or no product have this product code\n");
                                }
                            } while (!isQuoteExists);

                            Quote quoteData = getQuoteById(quoteList, quote_id);

                            printf("Is this the quote you wish to delete?[Y/N]\n");
                            printf("%3d %3d %-8s %-10s   %d\n", quoteData.quote_id, quoteData.productCode,
                                   quoteData.retailer, quoteData.status, quoteData.price);
                            scanf("%s", answer);
                            isYes = checkUserChoice(answer);

                            if (isYes > 0) {
                                deleteSingleQuote(quoteList, quote_id, productCode);
                            }
                            if (isYes < 0) {
                                printf("Invalid entry, please try again!!\n");
                                fprintf(log, "attempted quote delete at %s\n", ctime(&currentTime));
                                break;
                            }

                            if (isYes == 0) {
                                printf("Quote sheet remains unchanged!!\n");
                                fprintf(log, "discarded quote delete action at %s\n", ctime(&currentTime));
                                break;
                            }
                            showUpdatedQuotes(quoteList);
                            fprintf(log, "quote list updated at %s\n", ctime(&currentTime));
                            break;
                        case 2://remove one product; products that still has quotes cannot be deleted;
                            fprintf(log, "user chose to delete a product\n");
                            printf("Please enter a productCode:\n");
                            scanf("%d", &productCode);
                            isProductExists = searchByProductCode(productList, productCode);
                            if (!isProductExists) {
                                printf("This productCode doesn't exist, please try another one\n");
                                break;
                            }
                            Product productData = getProductByProductCode(productList, productCode);

                            printf("Is this the product you wish to delete?[Y/N]\n");
                            printf("%d\t%s\t%s\t%d\t%.2f\t%s\n", productData.productCode, productData.mark,
                                   productData.model, productData.ram, productData.screenSize,
                                   productData.operatingSystem);
                            scanf("%s", answer);
                            isYes = checkUserChoice(answer);
                            if (isYes == 0) {
                                printf("This product stays, please try another productCode.\n");
                                fprintf(log, "attempted product delete action at %s\n", ctime(&currentTime));
                                break;
                            }
                            if (isYes < 0) {
                                printf("Invalid entry, please try again!!\n");
                                fprintf(log, "discarded product delete action at %s\n", ctime(&currentTime));
                                break;
                            }
                            hasQuotes = checkForQuotes(quoteList, productCode);
                            if (hasQuotes) {
                                printf("This product still has some quotes, see below:\n");
                                showProduct(products, productCode);
                                printf("\nDo you wish to delete all the quotes under this product as well?[Y/N]\n");
                                scanf("%s", answer);
                                isYes = checkUserChoice(answer);
                                if (isYes == 0) {
                                    printf("This product stays and all the quotes under it, please try another productCode.\n");
                                    fprintf(log, "attempted product and quote delete action at %s\n",
                                            ctime(&currentTime));
                                    break;
                                }
                                if (isYes < 0) {
                                    printf("Invalid entry, please try again!!\n");
                                    fprintf(log, "discarded product and quote delete action at %s\n",
                                            ctime(&currentTime));
                                    break;
                                }
                                productList = deleteSingleProduct(productList, quoteList, productCode);
                                showUpdatedProducts(productList);
                                fprintf(log, "product and quote delete action at %s\n", ctime(&currentTime));
                                break;
                            } else {
                                productList = deleteSingleProduct(productList, quoteList, productCode);
                                showUpdatedProducts(productList);
                                fprintf(log, "product delete action at %s\n", ctime(&currentTime));
                                break;
                            }
                        default:
                            printf("Please try again!!\n");
                            fprintf(log, "Invalid entry\n");
                    }
                } while (deleteInput != 0);

            case 5://add an entry
                fprintf(log, "user chose to add an item\n");
                //user cannot add quote to a products moves productCode doesn't already exist
                //unless they are ready to add a new product that corresponds to the new quote
                printf("Do you wish to add a product to our listings?[Y/N]\n");
                scanf("%s", answer);
                isYes = checkUserChoice(answer);
                if (isYes < 0) {
                    printf("invalid entry, please try again!!\n");
                    fprintf(log, "attempted product add action at %s\n", ctime(&currentTime));
                    break;
                }
                if (isYes > 0) {
                    printf("Please enter a new productCode:\n");
                    scanf("%d", &productCode);
                    isExists = searchByProductCode(products, productCode);
                    if (isExists == true) {
                        //if productCode already exists then no need to duplicate it
                        //only possibility is to add new quote
                        printf("This productCode already exists\n");
                        printf("Do you wish to add a quote with this productCode?[Y/N]\n");
                        scanf("%s", answer);
                        isYes = checkUserChoice(answer);
                        if (isYes < 0) {
                            printf("invalid entry please try again!!\n");
                        }
                        if (isYes > 0) {
                            addNewQuote(quoteList, productCode);
                            fprintf(log, "quote add action at %s\n", ctime(&currentTime));
                        } else {
                            printf("This productCode already exists, please try another one\n");
                        }
                        break;
                    } else {
                        addNewProduct(productList, productCode);
                        fprintf(log, "product add action at %s\n", ctime(&currentTime));
                    }
                    break;
                } else {
                    do {
                        printf("Please enter the productCode for this new quote:\n");
                        scanf("%d", &productCode);
                        isExists = searchByProductCode(productList, productCode);
                        if (!isExists) { printf("This productCode doesn't exist, please try another one\n"); }
                    } while (!isExists);
                    addNewQuote(quoteList, productCode);
                    fprintf(log, "quote add action at %s\n", ctime(&currentTime));
                }
                break;
            case 0:
                writeChangesToFile(productList, quoteList);
                freeQuoteList(availablePhones);
                freeProductList(searchResult);
                freeProductList(productList);
                freeQuoteList(quoteList);
                freeProductList(products);
                freeQuoteList(quotes);

                fclose(log);
                return 0;
            default:
                printf("Please try again!!\n");
                fprintf(log, "Invalid entry\n");
        }

    } while (input);
}

void showProduct(productNode *products, int productCode) {
    while (products != NULL) {
        if (productCode == products->product.productCode) {
            printf("%d\t%s\t%s\t%d\t%.2f\t%s\n", products->product.productCode,
                   products->product.mark, products->product.model, products->product.ram,
                   products->product.screenSize, products->product.operatingSystem);
            if (products->quote != NULL) {
                printQuotesListAsc(products->quote);
            }
        }
        products = products->next;
    }
}

void showUpdatedProducts(productNode *productList) {
    printf("This product has successfully been deleted.\n");
    printf("Here's your updated product list.\n");
    printProductsListAsc(productList);
    printf("All changes will be commited to file as soon as you exit the program.\n");
}

void showUpdatedQuotes(quoteNode *quoteList) {
    printf("Here's the updated quote list.\n");
    printQuotesListAsc(quoteList);
    printf("All changes will be commited to file as soon as you exit the program.\n");
}

void writeChangesToFile(productNode *pTag, quoteNode *qTag) {
    FILE *productFile, *quoteFile;

    productFile = fopen("C:\\Users\\admin\\CLionProjects\\hw_3_Mine\\Products.txt", "w+");
    if (productFile == NULL) {
        printf("Error opening Products file.\n");
        free(productFile);
        fclose(productFile);
        exit(2);
    }

    quoteFile = fopen("C:\\Users\\admin\\CLionProjects\\hw_3_Mine\\Quotes.txt", "w+");
    if (quoteFile == NULL) {
        printf("Error opening Quotes file.\n");
        free(quoteFile);
        fclose(quoteFile);
        exit(3);
    }

    if (pTag) { printProductsToFile(pTag, productFile); }
    if (qTag) { printQuotesToFile(qTag, quoteFile); }

    fclose(quoteFile);
    fclose(productFile);
}

bool checkForQuotes(quoteNode *qTag, int code) {
    if (qTag != NULL) {
        if (code == qTag->quote.productCode) {
            return true;
        }
        return checkForQuotes(qTag->next, code);
    }
    return false;
}

void deleteSingleQuote(quoteNode *products, int quote_id, int productCode) {

    if (products == NULL) {
        printf("There's nothing to delete.\n");
        return;
    }
    quoteNode *tmp = NULL;
    bool isDeleted = false;

    if (productCode == products->quote.productCode && quote_id == products->quote.quote_id) {

        if (products->previous == NULL) {
            tmp = products;
            products = products->next;
            products->previous = NULL;
            free(tmp);
            resetQuoteIndexes(products);
            isDeleted = true;
        } else if (products->next == NULL) {
            tmp = products;
            products = products->previous;
            products->next = NULL;
            free(tmp);
            isDeleted = true;
        } else {
            tmp = products;
            tmp->previous->next = tmp->next;
            tmp->next->previous = tmp->previous;
            free(tmp);
            resetQuoteIndexes(products);
            isDeleted = true;
        }
    } else {
        deleteSingleQuote(products->next, quote_id, productCode);
    }

    if (isDeleted) {
        printf("The quote was successfully been deleted.\n");
    }
}

void resetQuoteIndexes(quoteNode *quotes) {
    if (quotes != NULL) {
        //if (quotes->next != NULL) {
        quotes->index = quotes->index - 1;
        //}
        resetQuoteIndexes(quotes->next);
    }//reset node indexes
}

void resetProductIndexes(productNode *products) {
    if (products != NULL) {
        products->index = products->index - 1;
        resetProductIndexes(products->next);
    }//reset node indexes
}

quoteNode *deleteQuotesInBulk(quoteNode *quotes, int productCode) {

    quoteNode *tmp = NULL;
    if (quotes == NULL) {
        return quotes;
    }

    if (productCode == quotes->quote.productCode) {
        if (quotes->previous == NULL) {
            tmp = quotes;
            quotes = quotes->next;
            quotes->previous = NULL;
            free(tmp);
            resetQuoteIndexes(quotes);
        } else if (quotes->next == NULL) {
            tmp = quotes;
            quotes = quotes->previous;
            quotes->next = NULL;
            free(tmp);
            return quotes;
        } else {
            tmp = quotes;
            tmp->previous->next = tmp->next;
            tmp->next->previous = tmp->previous;
            free(tmp);
            resetQuoteIndexes(quotes);
        }
    } else {
        return (quotes->next == NULL) ? quotes : deleteQuotesInBulk(quotes->next, productCode);
    }

    return deleteQuotesInBulk(quotes, productCode);
}

productNode *deleteSingleProduct(productNode *products, quoteNode *quotes, int productCode) {
    productNode *tmp = NULL;
    bool hasQuotes;
    while (products->next != NULL) {
        if (productCode == products->product.productCode) {
            hasQuotes = checkForQuotes(quotes, quotes->quote.productCode);
            if (hasQuotes) {
                quotes = deleteQuotesInBulk(quotes, productCode);
            }

            if (products->previous == NULL) {
                tmp = products;
                products = products->next;
                products->previous = NULL;
                free(tmp);
                resetProductIndexes(products);
            } else if (products->next == NULL) {
                products = products->previous;
                products->next = NULL;
                free(tmp);
            } else {
                tmp = products;
                tmp->previous->next = tmp->next;
                tmp->next->previous = tmp->previous;
                free(tmp);
                resetProductIndexes(products);
            }
        }
        products = products->next;
    }
    return products;
}

productNode *readProducts(productNode *products) {
    FILE *productFile;
    productNode *productList = NULL;
    Product productData;

    productFile = fopen("C:\\Users\\admin\\CLionProjects\\hw_3_Mine\\Products.txt", "r+");

    if (productFile == NULL) {
        printf("Error opening the Products file.\n");
        free(productFile);
        fclose(productFile);
        exit(100);
    }

    productList = getProductData(productFile, productData);
    products = productList;

    if (fclose(productFile) == EOF) {
        printf("Error closing the Products file\n");
        free(productFile);
        fclose(productFile);
        exit(300);
    }
    return products;
}

quoteNode *readQuotes(quoteNode *quotes) {
    FILE *quotesFile;
    quoteNode *quoteList = NULL;
    Quote quoteData;

    quotesFile = fopen("C:\\Users\\admin\\CLionProjects\\hw_3_Mine\\Quotes.txt", "r+");

    if (quotesFile == NULL) {
        printf("Error opening the Quotes file.\n");
        free(quotesFile);
        fclose(quotesFile);
        exit(200);
    }

    quoteList = getQuoteData(quotesFile, quoteData);
    quotes = quoteList;

    if (fclose(quotesFile) == EOF) {
        printf("Error closing the Quotes file\n");
        free(quotesFile);
        fclose(quotesFile);
        exit(900);
    }
    return quotes;
}

productNode *getProductData(FILE *productFile, Product productData) {
    productNode *list;

    if (!(list = (productNode *) malloc(sizeof(productNode)))) {
        printf("Memmory allocation failed\n");
        free(list);
        exit(400);
    }

    char mark[LENGTH_OF_CHAR];
    char model[LENGTH_OF_CHAR];
    char operatingSystem[LENGTH_OF_CHAR];
    int numberOfLines = 0;

    while (fscanf(productFile, "%d,%[^,],%[^,],%d,%f,%s", &productData.productCode, mark,
                  model, &productData.ram, &productData.screenSize, operatingSystem) == 6) {
        productData = allocProductData(mark, model, operatingSystem, productData);

//        printf("%4d %-8s %-10s   %4d   %.2f  %-9s\n", product.productCode, product.mark, product.model,
//               product.ram, product.screenSize, product.operatingSystem);

        numberOfLines += 1;

        if (numberOfLines == 1) {
            list = makeProductNode(productData);
        } else { list = appendProductNode(list, productData); }
    }

    return list;
}

quoteNode *getQuoteData(FILE *quoteFile, Quote quoteData) {

    quoteNode *quotesList;

    if (!(quotesList = (quoteNode *) malloc(sizeof(quoteNode)))) {
        printf("Memory allocation failed\n");
        free(quotesList);
        exit(400);
    }

    char retailer[LENGTH_OF_CHAR];
    char status[LENGTH_OF_CHAR];

    int numberOfLines = 0;

    while ((fscanf(quoteFile, "%d,%d,%[^,],%[^,],%d", &quoteData.quote_id, &quoteData.productCode, retailer,
                   status, &quoteData.price)) == 5) {
        quoteData = allocQuoteData(retailer, status, quoteData);

        //printf("%4d %4d %-8s %-10s %d \n", quote.quote_id, quote.productCode, quote.retailer, quote.status,
        //quote.price);

        numberOfLines += 1;

        if (numberOfLines == 1) {
            quotesList = makeQuoteNode(quoteData);
        } else {
            appendQuoteNode(quotesList, quoteData);
        }
    }

    return quotesList;
}

productNode *makeProductNode(Product productData) {

    productNode *newNode = NULL;
    newNode = (productNode *) malloc(sizeof(productNode));

    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        free(newNode);
        exit(500);
    }

    newNode->product = productData;
    newNode->next = NULL;
    newNode->previous = NULL;

    return newNode;
}

quoteNode *makeQuoteNode(Quote quoteData) {

    quoteNode *newNode = NULL;
    newNode = (quoteNode *) malloc(sizeof(quoteNode));

    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        free(newNode);
        exit(600);
    }

    newNode->quote = quoteData;
    newNode->next = NULL;
    newNode->previous = NULL;

    return newNode;
}

productNode *appendProductNode(productNode *head, Product productData) {
    if (head->next == NULL) {
        productNode *tmp = NULL;
        tmp = makeProductNode(productData);
        tmp->next = NULL;
        tmp->previous = head;
        head->next = tmp;
        if (head->previous == NULL) {
            head->index = 1;
        }
        if (head->previous != NULL) { head->index = head->previous->index + 1; }
        return head;
    }

    appendProductNode(head->next, productData);
    while (head->next != NULL) {
        head = head->next;
        head->index = head->previous->index + 1;
    }//setting index of last element

    while (head->previous != NULL) {
        head = head->previous;
    }//going back to head
    return head;
}

void appendQuoteNode(quoteNode *head, Quote quoteData) {

    if (head->next == NULL) {
        quoteNode *tmp = NULL;
        tmp = makeQuoteNode(quoteData);
        tmp->next = NULL;
        tmp->previous = head;
        head->next = tmp;
        if (head->previous == NULL) {
            head->index = 1;
        } else { head->index = head->previous->index + 1; }
        return;
    }

    appendQuoteNode(head->next, quoteData);
    while (head->next != NULL) {
        head = head->next;
        head->index = head->previous->index + 1;
    }//setting index of last element

    while (head->previous != NULL) {
        head = head->previous;
    }//going back to head
}

quoteNode *appendSingleQuoteNode(quoteNode *head, Quote quoteData) {
    if (head->next == NULL) {
        quoteNode *tmp = NULL;
        tmp = makeQuoteNode(quoteData);
        tmp->next = NULL;
        tmp->previous = head;
        head->next = tmp;
        return head;
    }
    return appendSingleQuoteNode(head->next, quoteData);
}

productNode *appendSingleProductNode(productNode *head, productNode *tail) {
    if (head->next == NULL) {
        productNode *tmp = NULL;
        tmp = tail;
        tmp->next = NULL;
        tmp->previous = head;
        head->next = tmp;
        return head;
    } else { return appendSingleProductNode(head->next, tail); }
}

quoteNode *sortQuotesById(quoteNode *head, quoteNode *tail) {

    if (head == NULL) {
        return tail;
    }

    if (tail->quote.productCode == head->quote.productCode) {
        tail = appendSingleQuoteNode(tail, head->quote);
        return (head->next == NULL) ? tail : sortQuotesById(head->next, tail);
    } else {
        return (head->next == NULL) ? tail : sortQuotesById(head->next, tail);
    }
}

productNode *changeProductNodeHead(productNode *head, productNode *nextHead) {
    while (head->next != NULL && nextHead != NULL) {
        if (head->product.productCode == nextHead->product.productCode) {
            nextHead = nextHead->next;
            changeProductNodeHead(head, nextHead);
        } else {
            return nextHead;
        }
    }
}

quoteNode *changeQuoteNodeHead(quoteNode *head, quoteNode *nextHead) {
    while (head->next != NULL && nextHead != NULL) {
        if (head->quote.productCode == nextHead->quote.productCode) {
            nextHead = nextHead->next;
            changeQuoteNodeHead(head, nextHead);
        } else {
            return nextHead;
        }
    }
}

productNode *addQuotesToProduct(productNode *products, quoteNode *quotes) {
    if (products == NULL) { return products; }
    //don't forget to handle the null scenario for incoming products and quotes
    if (products->product.productCode == quotes->quote.productCode) {
        products->quote = quotes;
    } else {
        products = changeProductNodeHead(products, products->next);
        addQuotesToProduct(products, quotes);
    }
    return products;
}

void constructMainList(productNode *fullProducts, quoteNode *fullQuotes) {

    if (fullProducts == NULL) { return; }//incase the list is empty

    quoteNode *truncatedRList = NULL;

    while (fullProducts->next != NULL) {
        truncatedRList = makeQuoteNode(fullQuotes->quote);
        truncatedRList = sortQuotesById(fullQuotes->next, truncatedRList);
        fullProducts = addQuotesToProduct(fullProducts, truncatedRList);
        fullQuotes = changeQuoteNodeHead(fullQuotes, fullQuotes->next);
        constructMainList(fullProducts, fullQuotes);
    }
}

void printProductsListAsc(productNode *products) {
    printf("\nProducts:\n");

    if (products == NULL) {
        printf("your list contains null\n");
    }

    while (products != NULL) {
        printf("%4d\t%s\t%s\t%4d\t%.2f\t%s\n", products->product.productCode, products->product.mark,
               products->product.model, products->product.ram, products->product.screenSize,
               products->product.operatingSystem);
        if (products->quote != NULL) {
            printQuotesListAsc(products->quote);
        }
        products = products->next;
    }

    printf("<End of List>\n\n");
}

void printQuotesListAsc(quoteNode *quotes) {

    printf("\nQuotes:\n");

    if (quotes == NULL) {
        printf("your list contains null\n");
    }

    while (quotes->previous != NULL) {
        quotes = quotes->previous;
    }

    while (quotes != NULL) {
        printf("%d\t%d\t%s\t%d\t%s\n", quotes->quote.quote_id, quotes->quote.productCode,
               quotes->quote.retailer, quotes->quote.price, quotes->quote.status);
        quotes = quotes->next;
    }

    printf("<End of List>\n\n");
}

void printProductsToFile(productNode *products, FILE *productFile) {
    while (products->previous != NULL) {
        products = products->previous;
    }//go back to head of linked-list

    rewind(productFile);

    while (products != NULL) {
        fprintf(productFile, "%d,%s,%s,%d,%.2f,%s\n", products->product.productCode, products->product.mark,
                products->product.model, products->product.ram, products->product.screenSize,
                products->product.operatingSystem);
        products = products->next;
    }
}

void printQuotesToFile(quoteNode *quotes, FILE *quoteFile) {
    while (quotes->previous != NULL) {
        quotes = quotes->previous;
    }

    rewind(quoteFile);
    while (quotes != NULL) {
        fprintf(quoteFile, "%d,%d,%s,%s,%d\n", quotes->quote.quote_id, quotes->quote.productCode,
                quotes->quote.retailer, quotes->quote.status, quotes->quote.price);
        quotes = quotes->next;
    }
}

bool searchByProductCode(productNode *products, int productCode) {
    while (products != NULL) {
        if (productCode == products->product.productCode) {
            return true;
        } else {
            return searchByProductCode(products->next, productCode);
        }
    }
    return false;
}

Product getProductByProductCode(productNode *products, int productCode) {
    while (products != NULL) {
        if (productCode == products->product.productCode) {
            return products->product;
        } else {
            return getProductByProductCode(products->next, productCode);
        }
    }
}

bool searchByQuoteId(quoteNode *quotes, int quote_id) {
    while (quotes != NULL) {
        if (quote_id == quotes->quote.quote_id) {
            return true;
        } else {
            return searchByQuoteId(quotes->next, quote_id);
        }
    }
    return false;
}

Quote getQuoteById(quoteNode *quotes, int quote_id) {
    while (quotes != NULL) {
        if (quote_id == quotes->quote.quote_id) {
            return quotes->quote;
        } else {
            return getQuoteById(quotes->next, quote_id);
        }
    }
}

int checkUserChoice(const char *answer) {
    int asciiValue = (int) answer[0];
    if (asciiValue == 89 || asciiValue == 121) {
        return 1;
    } else if (asciiValue == 78 || asciiValue == 110) { return 0; }
    else return -1;
}

quoteNode *editQuote(quoteNode *quotes, int productCode) {
    char retailer[LENGTH_OF_CHAR];
    char status[LENGTH_OF_CHAR];
    char answer[LENGTH_OF_CHAR];
    int quote_id;
    bool isQuoteExists, isYes;
    Quote quotesData, quoteFromList;

    do {
        printf("Please enter a quote_id:\n");
        scanf("%d", &quote_id);
        quoteFromList = getQuoteById(quotes, quote_id);
        isQuoteExists = productCode == quoteFromList.productCode;
        if (!isQuoteExists) {
            printf("This quote_id doesn't exist, or its product code doesn't match\n");
        }
    } while (!isQuoteExists);

    printf("Is this the quote you wish to change?[Y/N]\n");
    printf("%d\t%d\t%s\t%d\t%s\n", quoteFromList.quote_id, quoteFromList.productCode,
           quoteFromList.retailer, quoteFromList.price, quoteFromList.status);
    scanf("%s", answer);
    isYes = checkUserChoice(answer);

    if (!isYes) {
        printf("Then the quotes sheet remains unchanged!");
        return quotes;
    }

    quotesData.quote_id = quote_id;

    printf("Please enter the new product name:\n");
    scanf("%s", retailer);

    printf("Please enter the new status of this product:\n");
    scanf("%s", status);

    printf("Please enter the new quote:\n");
    scanf("%d", &quotesData.price);

    quotesData = allocQuoteData(retailer, status, quotesData);

    quotesData.productCode = productCode;

    printf("Is this the new quote you wish to post?[Y/N]\n");
    printf("%d\t%d\t%s\t%d\t%s\n", quotesData.quote_id, quotesData.productCode,
           quotesData.retailer, quotesData.price, quotesData.status);
    scanf("%s", answer);
    isYes = checkUserChoice(answer);
    if (isYes) {
        while (quotes != NULL) {
            if (quote_id == quotes->quote.quote_id) {
                quotes->quote = quotesData;
                return quotes;
            }
            quotes = quotes->next;
        }
    } else {
        printf("Then the quotes sheet remains unchanged!");
        return quotes;
    }
}

productNode *editProduct(productNode *products, int productCode) {

    char answer[LENGTH_OF_CHAR];
    bool isYes;

    Product product = getProductDataFromUser(productCode);

    printf("Is this the update you wish to make?[Y/N]\n");
    printf("%d\t%s\t%s\t%d\t%.2f\t%s\n", product.productCode, product.mark,
           product.model, product.ram, product.screenSize, product.operatingSystem);
    scanf("%s", answer);
    isYes = checkUserChoice(answer);

    if (isYes) {
        while (products != NULL) {
            if (productCode == products->product.productCode) {
                products->product = product;
                return products;
            }
            products = products->next;
        }
    }
    return products;
}

void addNewQuote(quoteNode *quotes, int productCode) {
    char retailer[LENGTH_OF_CHAR];
    char status[LENGTH_OF_CHAR];
    int quote_id;
    bool isQuoteExists;
    Quote quoteData;

    do {
        printf("Please enter a quote_id:\n");
        scanf("%d", &quote_id);
        isQuoteExists = searchByQuoteId(quotes, quote_id);
        if (isQuoteExists) { printf("This quote_id already exists, quote_id must be unique\n"); }
    } while (isQuoteExists);

    quoteData.quote_id = quote_id;

    printf("Please enter the username:\n");
    scanf("%s", retailer);

    printf("Please enter the status of this user:\n");
    scanf("%s", status);

    quoteData = allocQuoteData(retailer, status, quoteData);

    quoteData.productCode = productCode;

    while (quotes != NULL) {
        if (productCode != quotes->quote.productCode) {
            while (quotes->next != NULL) {
                quotes = quotes->next;
            }
            quotes = appendSingleQuoteNode(quotes, quoteData);
        }
        quotes = quotes->next;
    }
}

Quote allocQuoteData(char *retailer, char *status, Quote quoteData) {
    quoteData.retailer = malloc((strlen(retailer) + 1) * sizeof(char *));
    if (!quoteData.retailer) {
        free(quoteData.retailer);
        exit(7);
    }
    strcpy(quoteData.retailer, retailer);

    quoteData.status = malloc((strlen(status) + 1) * sizeof(char *));
    if (!quoteData.status) {
        free(quoteData.status);
        exit(8);
    }
    strcpy(quoteData.status, status);

    return quoteData;
}

void addNewProduct(productNode *products, int productCode) {
    char mark[LENGTH_OF_CHAR];
    char model[LENGTH_OF_CHAR];
    char operatingSystem[LENGTH_OF_CHAR];
    productNode *tmp;
    Product productData = getProductDataFromUser(productCode);

    productData = allocProductData(mark, model, operatingSystem, productData);

    tmp->product = productData;
    while (products->next != NULL) {
        if (products->product.productCode >= products->next->product.productCode) {
            products = products->next;
        }//if productCode larger swap go to next node
        else {
            tmp->next = products->next->next;
            products->next = tmp;
            tmp->previous = products;
            tmp->next->previous = tmp;
        }
    }
}

Product getProductDataFromUser(int productCode) {
    char mark[LENGTH_OF_CHAR];
    char model[LENGTH_OF_CHAR];
    char operatingSystem[LENGTH_OF_CHAR];
    Product productData;

    productData.productCode = productCode;

    printf("Please enter the phone maker:\n");
    scanf("%s", mark);

    printf("Please enter the phone model:\n");
    scanf("%s", model);

    printf("Please enter the phone ram:\n");
    scanf("%d", &productData.ram);

    printf("Please enter the screen size of the phone:\n");
    scanf("%f", &productData.screenSize);

    printf("Please enter the phone's operating system:\n");
    scanf("%s", operatingSystem);

    productData = allocProductData(mark, model, operatingSystem, productData);

    return productData;
}

Product allocProductData(char *mark, char *model, char *operatingSystem,
                         Product productData) {
    productData.mark = calloc(strlen(mark) + 1, sizeof(char));
    if (!productData.mark) {
        free(productData.mark);
        exit(9);
    }
    strcpy(productData.mark, mark);

    productData.model = calloc(strlen(model) + 1, sizeof(char));
    if (!productData.model) {
        free(productData.model);
        exit(10);
    }
    strcpy(productData.model, model);

    productData.operatingSystem = calloc(strlen(operatingSystem) + 1, sizeof(char));
    if (!productData.operatingSystem) {
        free(productData.operatingSystem);
        exit(11);
    }
    strcpy(productData.operatingSystem, operatingSystem);

    return productData;
}

void bubbleSortQuoteList(quoteNode *head) {
    int swapped;
    quoteNode *firstNode;

    //making sure list is not empty
    if (head == NULL) { return; }

    //do bubblesort
    do {
        swapped = 0;
        firstNode = head;
        while (firstNode->next != NULL) {
            if (firstNode->quote.productCode > firstNode->next->quote.productCode) {
                swap(firstNode, firstNode->next);
                swapped = 1;
            }
            firstNode = firstNode->next;
        }
    } while (swapped);
}

void swap(quoteNode *quote, quoteNode *nextQuote) {
    Quote temp = quote->quote;
    quote->quote = nextQuote->quote;
    nextQuote->quote = temp;
}

quoteNode *getAvailablePhones(quoteNode *quotes) {
    quoteNode *phonesInStock = NULL;
    while (quotes != NULL) {
        char inStock[LENGTH_OF_CHAR] = {"in-stock"};
        if (strcmp(quotes->quote.status, inStock) == 0) {
            if (phonesInStock == NULL) {
                phonesInStock = makeQuoteNode(quotes->quote);
            } else {
                phonesInStock = appendSingleQuoteNode(phonesInStock, quotes->quote);
            }
        }
        quotes = quotes->next;
    }
    return phonesInStock;
}

productNode *getPhonesByMaker(productNode *products, char *userInput) {
    productNode *phones = NULL;
    char *phoneMaker;
    while (products != NULL) {

        for (int i = 0; i < strlen(products->product.mark); i++) {
            products->product.mark[i] = tolower(products->product.mark[i]);
        }
        phoneMaker = calloc(strlen(products->product.mark) + 1, sizeof(char));
        if (!phoneMaker) {
            free(phoneMaker);
            exit(10);
        }
        strcpy(phoneMaker, products->product.mark);
        if (strcmp(userInput, phoneMaker) == 0) {
            if (phones == NULL) {
                phones = makeProductNode(products->product);
                phones->quote = products->quote;
                phones->next = NULL;
                phones->previous = NULL;
            } else { phones = appendSingleProductNode(phones, products); }
        }
        products = products->next;
    }
    return phones;
}

productNode *getPhonesByModel(productNode *products, char *userInput) {
    productNode *phones = NULL;
    char *phoneModel;
    while (products != NULL) {
        for (int i = 0; i < strlen(products->product.model); i++) {
            products->product.model[i] = tolower(products->product.model[i]);
        }
        phoneModel = calloc(strlen(products->product.model) + 1, sizeof(char));
        if (!phoneModel) {
            free(phoneModel);
            exit(10);
        }
        strcpy(phoneModel, products->product.model);
        if (strcmp(userInput, phoneModel) == 0) {
            if (phones == NULL) {
                phones = makeProductNode(products->product);
                phones->quote = products->quote;
                phones->next = NULL;
                phones->previous = NULL;
            } else {
                phones = appendSingleProductNode(phones, products);
            }
        }
        products = products->next;
    }
    return phones;
}

void freeProductList(productNode *product) {
    productNode *nexthead = NULL;
    while (product != NULL) {
        nexthead = product->next;
        if (product->quote != NULL) { freeQuoteList(product->quote); }
        free(product->product.model);
        free(product->product.mark);
        free(product->product.operatingSystem);
        free(product);
        product = nexthead;
    }
}

void freeQuoteList(quoteNode *quote) {
    quoteNode *nextHead = NULL;
    while (quote != NULL) {
        nextHead = quote->next;
        free(quote->quote.status);
        free(quote->quote.retailer);
        free(quote);
        quote = nextHead;
    }
}