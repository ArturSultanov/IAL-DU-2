/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
	(*tree) = NULL;

}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
	if(tree == NULL){
		return false;
	}

	// If key is found, return true and store the value.
	if(tree->key == key){
		*value = tree->value;
		return true;

	// If key is smaller than the current node, search the left subtree.
	} else if(key < tree->key){
		return bst_search(tree->left, key, value);

	// If key is bigger than the current node, search the right subtree.
	} else {
		return bst_search(tree->right, key, value);
	}
	
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
	// If 'tree' pointer don't point to anything, return.
	if(tree == NULL){
		return;
	}

	
	if((*tree) == NULL){ // If the tree is empty.
		// Allocate memory for inserting node.
		bst_node_t *insert_node = (bst_node_t *) malloc(sizeof(bst_node_t));

		if(insert_node == NULL){ // Allocation failed.
			return;
		}

		insert_node->key = key;
		insert_node->value = value;
		insert_node->left = NULL;
		insert_node->right = NULL;
		(*tree) = insert_node;

	} else {
		// If the key is equal to the current node, replace the value.
		if((*tree)->key == key){
			(*tree)->value = value;

		// If the key is smaller than the current node, insert it to the left subtree.
		} else if(key < (*tree)->key){
			bst_insert(&((*tree)->left), key, value);

		// If the key is bigger than the current node, insert it to the right subtree.
		} else {
			bst_insert(&((*tree)->right), key, value);
		}

	}
}
/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
	if((*tree) == NULL){
		return;
	}

	if((*tree)->right == NULL){ // Found the rightmost node.
		target->key = (*tree)->key;
		target->value = (*tree)->value;
		// Store the left subtree of the current node.
		bst_node_t *delete_node = (*tree);
		(*tree) = (*tree)->left;
		free(delete_node);

	// If the right subtree is not empty, search the right subtree.
	} else {
		bst_replace_by_rightmost(target, &((*tree)->right));
	}
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (tree == NULL) {
        return;
    }
    if ((*tree) == NULL) {
        return;
    }

	if ((*tree)->key == key) { // We found a node with target key.

		// The node has two children.
		if ((*tree)->left == NULL && (*tree)->right == NULL) {
			free((*tree));
			(*tree) = NULL;

		// The node has only right child.
		} else if ((*tree)->left == NULL) {
			bst_node_t *delete_node = (*tree);
			// Point the parent pointer to the olny child.
			(*tree) = (*tree)->right;
			// Free memory of the deleted node.
			free(delete_node);

		// The node has only left child.
		} else if ((*tree)->right == NULL) {
			bst_node_t *delete_node = (*tree);
			// Point the parent pointer to the olny child.
			(*tree) = (*tree)->left;
			// Free memory of the deleted node.
			free(delete_node);

		// The node has both (left and right) children.
		} else {
			// Replace the node with the rightmost node of the left subtree,
			// as it's written in the function description.
			bst_replace_by_rightmost((*tree), &((*tree)->left));
		}

	// If the key is smaller than the current node, search the left subtree.
	} else if (key < (*tree)->key) {
		bst_delete(&((*tree)->left), key);

	// If the key is bigger than the current node, search the right subtree.
	} else {
		bst_delete(&((*tree)->right), key);
	}
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    if (tree == NULL) {
        return;
    }
    if ((*tree) == NULL) {
        return;
    }

	// Delete the left subtree.
	bst_dispose(&((*tree)->left));
	// Delete the right subtree.
	bst_dispose(&((*tree)->right));
	// Delete the current node.
	free((*tree));
	(*tree) = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
	// If the tree is empty, return.
	if(tree == NULL){
		return;
	}
	// root->left->right
	bst_add_node_to_items(tree, items);
	bst_preorder(tree->left, items);
	bst_preorder(tree->right, items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
	// If the tree is empty, return.
	if(tree == NULL){
		return;
	}
	// left->root->right
	bst_inorder(tree->left, items);
	bst_add_node_to_items(tree, items);
	bst_inorder(tree->right, items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
	// If the tree is empty, return.
	if(tree == NULL){
		return;
	}
	// left->right->root
	bst_postorder(tree->left, items);
	bst_postorder(tree->right, items);
	bst_add_node_to_items(tree, items);
}
