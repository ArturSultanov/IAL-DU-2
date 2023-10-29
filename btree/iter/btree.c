/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

	if (tree == NULL){
		return false;
	}

	bst_node_t *current_node = tree; 

	while (current_node != NULL){
		if (current_node->key == key){
			// Succses. Current node is the node we looking for.
			*value = current_node->value; // Store the note value to '*value' pointer.
			return true;

		} else if (current_node->key < key){ 	// If looking key is greater than current,
			current_node = current_node->right;	// go to the right branch.

		} else {
			current_node = current_node->left;	// Then go to the left branch.
		}		
	}

	return false;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if (*tree == NULL) {
        return;
    }

	bool node_found = false;
	bst_node_t **current_node = tree;

	while ((*current_node) != NULL)
	{
		if ((*current_node)->key == key) // Node with the same key is already exist.
		{
			(*current_node)->value = value; // Set new value to the Node.
			node_found = true;
			break;

		} else if ((*current_node)->key < key)
		{
			current_node = &((*current_node)->right);
		} else
		{
			current_node = &((*current_node)->left);
		}
	}

	if (node_found == false){ // The node with looking kay was not found.
		/*  New structure of abstract tree '**tree'.
				 root
				 /  \
			   ln	rn
			  /  \
		    lln  INST(*current_node)
				 /  \
			  NUll  NULL
		*/

		bst_node_t *insert_node = (bst_node_t *) malloc(sizeof(bst_node_t));
		if (insert_node == NULL){
			//return 1;
			return;
		}

		insert_node->key = key;
		insert_node->value = value;
		insert_node->left = NULL;
		insert_node->right = NULL;
		// After while loop (*current_node) == NULL
		(*current_node) = insert_node; // Set current node pointer to the new leaf.
		
	}
	return;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
	
	bst_node_t **rightmost_node = tree;
	while ((*rightmost_node)->right != NULL){
		rightmost_node = &((*rightmost_node)->right);
	}
	// Store the rightmost node values to target node.
	target->key = (*rightmost_node)->key;
	target->value = (*rightmost_node)->value;

	// Save the rightmost node address to delete_node pointer.
	bst_node_t *delete_node = *rightmost_node;
	*rightmost_node = (*rightmost_node)->left;
	free(delete_node);
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
	if (tree == NULL) { // Pointer to the tree is empty.
        return;
    }
    if ((*tree) == NULL) { // Tree is empty.
        return;
    }

	bst_node_t **current_node = tree;
	
	// Looking for the node with the same key we want to delete.
	while ((*current_node) != NULL){
		if ((*current_node)->key == key){
			break;
		} else if ((*current_node)->key < key){
			current_node = &((*current_node)->right);
		} else {
			current_node = &((*current_node)->left);
		}
	}

	if ((*current_node) == NULL){ // Node with the same key was not found.
		return; // Nothing to delete.
	}

	// Or the 'current_node' has the same key, that we want to delete.
	// First check if the node is leaf (has no children).
	if ((*current_node)->left == NULL && (*current_node)->right == NULL){
		free((*current_node));
		(*current_node) = NULL;

	// If the node has both children, replace it by the rightmost node of the left subtree.
    } else if ((*current_node)->left != NULL && (*current_node)->right != NULL){
		bst_replace_by_rightmost((*current_node), &((*current_node)->left));

	// If the node has only one (right) child, replace it by the child.
	} else if ((*current_node)->left == NULL){
		bst_node_t *delete_node = *current_node;
		(*current_node) = (*current_node)->right;
		free(delete_node);

	// If the node has only one (left) child, replace it by the child.
	} else {
		bst_node_t *delete_node = *current_node;
		(*current_node) = (*current_node)->left;
		free(delete_node);
	}
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
	if (tree == NULL) {
		return;
	}
	if ((*tree) == NULL) {
		return;
	}

	stack_bst_t stack; // Stack of nodes.
	stack_bst_init(&stack);

	while ((*tree) != NULL || (!stack_bst_empty(&stack))){
		// If the current node is not NULL, push it to the stack and go to the right branch.
		if ((*tree) != NULL){
			// Push the right branch to the stack.
			if ((*tree)->right != NULL){
				stack_bst_push(&stack, (*tree)->right);
			}
			bst_node_t *delete_node = (*tree);
			// Go to the left branch.
			(*tree) = (*tree)->left;
			free(delete_node);
		// If the current node is NULL and the stack is not empty, pop the node from the stack.
		} else {
			if (!stack_bst_empty(&stack)){
				(*tree) = stack_bst_top(&stack);
				stack_bst_pop(&stack);
			} else {
				break;
			}
		}
		
		// if ((*tree) == NULL){
		// 	if (!stack_bst_empty(&stack)){
		// 		(*tree) = stack_bst_top(&stack);
		// 		stack_bst_pop(&stack);
		// 	} else {
		// 		break;
		// 	}
		// } else {
		// 	if ((*tree)->right != NULL){
		// 		stack_bst_push(&stack, (*tree)->right);
		// 	}
		// 	bst_node_t *tmp = (*tree);
		// 	(*tree) = (*tree)->left;
		// 	free(tmp);
		// }

	}
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
	// Go to the leftmost node of the tree,
	// pushing all noded we went though to the stack
	// and adding them (pointers) to the items.

	while (tree != NULL)
	{
		bst_add_node_to_items(tree, items);
		stack_bst_push(to_visit, tree); // Save the node to the stack.
		tree = tree->left;
	}
	
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
	// root -> left -> right

	stack_bst_t stack; // Stack of nodes.
	stack_bst_init(&stack);

	bst_leftmost_preorder(tree, &stack, items);

	// While the stack is not empty, go to the right branch of the node.
	while (!stack_bst_empty(&stack))
	{
		tree = stack_bst_top(&stack);
		stack_bst_pop(&stack);
		bst_leftmost_preorder(tree->right, &stack, items);
	}
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
	// Go to the leftmost node of the tree,
	// pushing all noded we went though to the stack,
	// but not saving pointers to any output structure.

	while (tree != NULL){
		stack_bst_push(to_visit, tree);
		tree = tree->left;
	}
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
	// left -> root -> right

	stack_bst_t stack; // Stack of nodes.
	stack_bst_init(&stack);

	bst_leftmost_inorder(tree, &stack);

	while (!stack_bst_empty(&stack))
	{
		tree = stack_bst_top(&stack);
		stack_bst_pop(&stack);
		bst_add_node_to_items(tree, items);
		bst_leftmost_inorder(tree->right, &stack);
	}
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
	while (tree != NULL){
		stack_bst_push(to_visit, tree);
		stack_bool_push(first_visit, true);
		tree = tree->left;
	}
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
	// left -> right -> root

	// Stack of nodes.
	stack_bst_t stack;
    stack_bst_init(&stack);

	// Stack of bool values, if the node was visited.
    stack_bool_t stack_go_from_left;
    stack_bool_init(&stack_go_from_left);

    bst_leftmost_postorder(tree, &stack, &stack_go_from_left);

	bool go_from_left;

	while (!stack_bst_empty(&stack))
	{
		tree = stack_bst_top(&stack);
		stack_bst_pop(&stack);
		
		go_from_left = stack_bool_top(&stack_go_from_left);
		stack_bool_pop(&stack_go_from_left);

		if (go_from_left == true)
		{
			stack_bst_push(&stack, tree);
			stack_bool_push(&stack_go_from_left, false);
			bst_leftmost_postorder(tree->right, &stack, &stack_go_from_left);
		}
		else
		{
			bst_add_node_to_items(tree, items);
		}
	}
}
