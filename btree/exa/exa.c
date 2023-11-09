/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    // Initialize the tree.
    bst_init(tree);

    // Iterate over the input string.
    for (int symbol = 0; input[symbol] != '\0'; symbol++)
    {
        char key;
        
        if ((input[symbol] >= 'a' && input[symbol] <= 'z')|| // Check if the symbol is lowercase letter.
            (input[symbol] >= '0' && input[symbol] <= '9')|| // Or the symbol is a number.
            (input[symbol] == ' '))                          // or the symbol is a space.
        {
            key = input[symbol];
        }
        // Check if the symbol is uppercase letter.
        else if  (input[symbol] >= 'A' && input[symbol] <= 'Z')
        {
            key = input[symbol] + 32;
        }
        // Check if the letter is a special character.
        else
        {
            key = '_';
        }
        
        // Search for the key in the tree.
        int value = 0;
        if (bst_search(*tree, key, &value))
        {
            // If the key is in the tree, increment the value.
            value++;
            bst_insert(tree, key, value);
        }
        else
        {
            // If the key is not in the tree, insert it.
            bst_insert(tree, key, 1);
        }
    }
}



/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/

int bst_count_nodes(bst_node_t *node) {
    if (node == NULL) {
        return 0;
    }
    return bst_count_nodes(node->left) + bst_count_nodes(node->right) + 1;
}

void bst_to_array(bst_node_t *node, bst_items_t *items) {
    // Base case
    if (node == NULL)
        return;
    
    // Recursively convert the left subtree
    bst_to_array(node->left, items);
    
    // Add the current node to the array of items
    if (items->size < items->capacity) {
        items->nodes[items->size++] = node;
    }
    
    // Recursively convert the right subtree
    bst_to_array(node->right, items);
}

bst_node_t* array_to_bst(int start, int end, bst_node_t **nodes) {
    // Base case
    if (start > end)
        return NULL;
    
    // Get the middle element and make it root
    int mid = (start + end) / 2;
    bst_node_t *root = nodes[mid];
    
    // Recursively construct the left subtree
    root->left = array_to_bst(start, mid - 1, nodes);
    
    // Recursively construct the right subtree
    root->right = array_to_bst(mid + 1, end, nodes);
    
    return root;
}

void bst_balance(bst_node_t **tree) {
    if (tree == NULL)
    {
        return;
    }
    if (*tree == NULL)
    {
        return;
    }
    
    // Count the total number of nodes in the tree.
    int total_nodes = bst_count_nodes(*tree);

    // Initialize the array to hold the nodes.
    bst_items_t items;
    // bst_init_items(&items);

    items.capacity = total_nodes;
    items.size = 0;
    items.nodes = malloc(items.capacity * sizeof(bst_node_t*));
    
    // Memory allocation error.
    if (items.nodes == NULL) {
        return;
    }
    
    // Convert the BST to a sorted array of nodes
    bst_to_array(*tree, &items);
    
    // Build a balanced BST from the sorted array of nodes
    *tree = array_to_bst(0, items.size - 1, items.nodes);
    
    // Free the temporary array memory
    free(items.nodes);
}
