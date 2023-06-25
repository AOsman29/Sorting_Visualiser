#include<SDL.h>
#include<iostream>
#include<limits>
#include<time.h>
#include<string>
using namespace std;
#include<chrono>
#include<bits/stdc++.h>

const int SCREEN_WIDTH = 910;
const int SCREEN_HEIGHT = 750;

const int arrSize = 130;
const int rectSize = 7;

int arr[arrSize];
int Barr[arrSize];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool complete = false;

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError();
        success = false;
    }
    else
    {
        if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout << "Warning: Linear Texture Filtering not enabled.\n";
        }

        window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            cout << "Couldn't create window. SDL_Error: " << SDL_GetError();
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError();
                success = false;
            }
        }
    }

    return success;
}

void close()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void visualize(int x = -1, int y = -1, int z = -1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j = 0;
    for (int i = 0; i <= SCREEN_WIDTH - rectSize; i += rectSize)
    {
        SDL_PumpEvents();

        SDL_Rect rect = { i, 0, rectSize, arr[j] };
        if (complete)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if (j == x || j == z)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (j == y)
        {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

void inplaceHeapSort(int* input, int n)
{
    for (int i = 1; i < n; i++)
    {
        int childIndex = i;
        int parentIndex = (childIndex - 1) / 2;

        while (childIndex > 0)
        {
            if (input[childIndex] > input[parentIndex])
            {
                int temp = input[parentIndex];
                input[parentIndex] = input[childIndex];
                input[childIndex] = temp;

            }
            else
            {
                break;
            }

            visualize(parentIndex, childIndex);
            SDL_Delay(40);

            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
    }

    for (int heapLast = n - 1; heapLast >= 0; heapLast--)
    {
        int temp = input[0];
        input[0] = input[heapLast];
        input[heapLast] = temp;

        int parentIndex = 0;
        int leftChildIndex = 2 * parentIndex + 1;
        int rightChildIndex = 2 * parentIndex + 2;

        while (leftChildIndex < heapLast)
        {
            int maxIndex = parentIndex;

            if (input[leftChildIndex] > input[maxIndex])
            {
                maxIndex = leftChildIndex;
            }
            if (rightChildIndex<heapLast && input[rightChildIndex]>input[maxIndex])
            {
                maxIndex = rightChildIndex;
            }
            if (maxIndex == parentIndex)
            {
                break;
            }

            int temp = input[parentIndex];
            input[parentIndex] = input[maxIndex];
            input[maxIndex] = temp;

            visualize(maxIndex, parentIndex, heapLast);
            SDL_Delay(40);

            parentIndex = maxIndex;
            leftChildIndex = 2 * parentIndex + 1;
            rightChildIndex = 2 * parentIndex + 2;
        }
    }
}

int partition_array(int a[], int si, int ei)
{
    int count_small = 0;

    for (int i = (si + 1); i <= ei; i++)
    {
        if (a[i] <= a[si])
        {
            count_small++;
        }
    }
    int c = si + count_small;
    int temp = a[c];
    a[c] = a[si];
    a[si] = temp;
    visualize(c, si);

    int i = si, j = ei;

    while (i<c && j>c)
    {
        if (a[i] <= a[c])
        {
            i++;
        }
        else if (a[j] > a[c])
        {
            j--;
        }
        else
        {
            int temp_1 = a[j];
            a[j] = a[i];
            a[i] = temp_1;

            visualize(i, j);
            SDL_Delay(70);

            i++;
            j--;
        }
    }
    return c;
}

void quickSort(int a[], int si, int ei)
{
    if (si >= ei)
    {
        return;
    }

    int c = partition_array(a, si, ei);
    quickSort(a, si, c - 1);
    quickSort(a, c + 1, ei);

}

void merge2SortedArrays(int a[], int si, int ei)
{
    int size_output = (ei - si) + 1;
    int* output = new int[size_output];

    int mid = (si + ei) / 2;
    int i = si, j = mid + 1, k = 0;
    while (i <= mid && j <= ei)
    {
        if (a[i] <= a[j])
        {
            output[k] = a[i];
            visualize(i, j);
            i++;
            k++;
        }
        else
        {
            output[k] = a[j];
            visualize(i, j);
            j++;
            k++;
        }

    }
    while (i <= mid)
    {
        output[k] = a[i];
        visualize(-1, i);
        i++;
        k++;
    }
    while (j <= ei)
    {
        output[k] = a[j];
        visualize(-1, j);
        j++;
        k++;
    }
    int x = 0;
    for (int l = si; l <= ei; l++)
    {
        a[l] = output[x];
        visualize(l);
        SDL_Delay(15);
        x++;
    }
    delete[]output;
}

void mergeSort(int a[], int si, int ei)
{
    if (si >= ei)
    {
        return;
    }
    int mid = (si + ei) / 2;

    mergeSort(a, si, mid);
    mergeSort(a, mid + 1, ei);

    merge2SortedArrays(a, si, ei);
}

void bubbleSort()
{
    for (int i = 0; i < arrSize - 1; i++)
    {
        for (int j = 0; j < arrSize - 1 - i; j++)
        {
            if (arr[j + 1] < arr[j])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                visualize(j + 1, j, arrSize - i);
            }
            SDL_Delay(1);
        }
    }
}

void insertionSort()
{
    for (int i = 1; i < arrSize; i++)
    {
        int j = i - 1;
        int temp = arr[i];
        while (j >= 0 && arr[j] > temp)
        {
            arr[j + 1] = arr[j];
            j--;

            visualize(i, j + 1);
            SDL_Delay(5);
        }
        arr[j + 1] = temp;
    }
}

void selectionSort()
{
    int minIndex;
    for (int i = 0; i < arrSize - 1; i++)
    {
        minIndex = i;
        for (int j = i + 1; j < arrSize; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
                visualize(i, minIndex);
            }
            SDL_Delay(1);
        }
        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
}
void insertionSort1(int arr[], int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int temp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > temp)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

// Merge function merges the sorted runs
void merge(int arr[], int l, int m, int r)
{

    // Original array is broken in two parts
    // left and right array
    int len1 = m - l + 1, len2 = r - m;
    int* left = new int[len1], * right = new int[len2];
    for (int i = 0; i < len1; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < len2; i++)
        right[i] = arr[m + 1 + i];

    int i = 0;
    int j = 0;
    int k = l;

    // After comparing, we
    // merge those two array
    // in larger sub array
    while (i < len1 && j < len2)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of left, if any
    while (i < len1)
    {
        arr[k] = left[i];
        k++;
        i++;
    }

    // Copy remaining element of right, if any
    while (j < len2)
    {
        arr[k] = right[j];
        k++;
        j++;
        
    }
}

// Iterative Timsort function to sort the
// array[0...n-1] (similar to merge sort)
void timSort(int arr[], int n)
{

    // Sort individual subarrays of size RUN
    for (int i = 0; i < n; i += arrSize)
        insertionSort1(arr, i, min((i + arrSize - 1),
            (n - 1)));
   
    
   
    // Start merging from size RUN (or 32).
    // It will merge
    // to form size 64, then 128, 256
    // and so on ....
    for (int size = arrSize; size < n;
        size = 2 * size)
    {

        // pick starting point of
        // left sub array. We
        // are going to merge
        // arr[left..left+size-1]
        // and arr[left+size, left+2*size-1]
        // After every merge, we
        // increase left by 2*size
        for (int left = 0; left < n;
            left += 2 * size)
        {

            // find ending point of
            // left sub array
            // mid+1 is starting point
            // of right sub array
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1),
                (n - 1));

            // merge sub array arr[left.....mid] &
            // arr[mid+1....right]
            if (mid < right)
                merge(arr, left, mid, right);
            SDL_Delay(5);
            
        }
    }
}
int shellSort(int arr[], int n)
{
    // Start with a big gap, then reduce the gap
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        // Do a gapped insertion sort for this gap size.
        // The first gap elements a[0..gap-1] are already in gapped order
        // keep adding one more element until the entire array is
        // gap sorted 
        for (int i = gap; i < n; i += 1)
        {
            // add a[i] to the elements that have been gap sorted
            // save a[i] in temp and make a hole at position i
            int temp = arr[i];

            // shift earlier gap-sorted elements up until the correct 
            // location for a[i] is found
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];

            //  put temp (the original a[i]) in its correct location
            arr[j] = temp;
            visualize(i, j - 1);
            SDL_Delay(5);
        }
    }
    return 0;
}
struct Node
{
    int key;
    struct Node* left, * right;
};

// A utility function to create a new BST Node
struct Node* newNode(int item)
{
    struct Node* temp = new Node;
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

// Stores inorder traversal of the BST
// in arr[]
void storeSorted(Node* root, int arr[], int& i)
{
    if (root != NULL)
    {
        storeSorted(root->left, arr, i);
        arr[i++] = root->key;
        storeSorted(root->right, arr, i);
    }
}

/* A utility function to insert a new
   Node with given key in BST */
Node* insert(Node* node, int key)
{
    /* If the tree is empty, return a new Node */
    if (node == NULL) return newNode(key);

    /* Otherwise, recur down the tree */
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    /* return the (unchanged) Node pointer */
    return node;
}

// This function sorts arr[0..n-1] using Tree Sort
void treeSort(int arr[], int n)
{
    struct Node* root = NULL;

    // Construct the BST
    root = insert(root, arr[0]);
    for (int i = 1; i < n; i++)
        root = insert(root, arr[i]);
   
    // Store inorder traversal of the BST
    // in arr[]
    int i = 0;
    storeSorted(root, arr, i);
    visualize(i, arr[i]);
    SDL_Delay(5);

}
void cycleSort(int arr[], int n)
{
    // count number of memory writes
    int writes = 0;

    // traverse array elements and put it to on
    // the right place
    for (int cycle_start = 0; cycle_start <= n - 2; cycle_start++)
    {
        // initialize item as starting point
        int item = arr[cycle_start];

        // Find position where we put the item. We basically
        // count all smaller elements on right side of item.
        int pos = cycle_start;
        for (int i = cycle_start + 1; i < n; i++)
            if (arr[i] < item)
                pos++;
       
        // If item is already in correct position
        if (pos == cycle_start)
            continue;

        // ignore all duplicate  elements
        while (item == arr[pos])
            pos += 1;

        // put the item to it\'s right position
        if (pos != cycle_start)
        {
            swap(item, arr[pos]);
            writes++;

        }

        // Rotate rest of the cycle
        while (pos != cycle_start)
        {
            pos = cycle_start;

            // Find position where we put the element
            for (int i = cycle_start + 1; i < n; i++)
                if (arr[i] < item)
                    pos += 1;

            // ignore all duplicate  elements
            while (item == arr[pos])
                pos += 1;

            // put the item to it\'s right position
            if (item != arr[pos])
            {
                swap(item, arr[pos]);
                writes++;
                visualize(writes, pos);
                SDL_Delay(5);
            }

        }
    }

    // Number of memory writes or swaps
    // cout << writes << endl ;
}
/*void strandSort(list<int>& ip, list<int>& op)
{
    // Base case : input is empty
    if (ip.empty())
        return;

    // Create a sorted sublist with
    // first item of input list as
    // first item of the sublist
    list<int> sublist;
    sublist.push_back(ip.front());
    ip.pop_front();

    // Traverse remaining items of ip list
    for (auto it = ip.begin(); it != ip.end(); ) {

        // If current item of input list
        // is greater than last added item
        // to sublist, move current item
        // to sublist as sorted order is
        // maintained.
        if (*it > sublist.back()) {
            sublist.push_back(*it);

            // erase() on list removes an
            // item and returns iterator to
            // next of removed item.
            it = ip.erase(it);
        }

        // Otherwise ignore current element
        else
            it++;
    }

    // Merge current sublist into output
    op.merge(sublist);

    // Recur for remaining items in
    // input and current items in op.
    strandSort(ip, op);

}
*/
void CocktailSort(int a[], int n)
{
    bool swapped = true;
    int start = 0;
    int end = n - 1;

    while (swapped) {
        // reset the swapped flag on entering
        // the loop, because it might be true from
        // a previous iteration.
        swapped = false;

        // loop from left to right same as
        // the bubble sort
        for (int i = start; i < end; ++i) {
            if (a[i] > a[i + 1]) {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }

        // if nothing moved, then array is sorted.
        if (!swapped)
            break;

        // otherwise, reset the swapped flag so that it
        // can be used in the next stage
        swapped = false;

        // move the end point back by one, because
        // item at the end is in its rightful spot
        --end;

        // from right to left, doing the
        // same comparison as in the previous stage
        for (int i = end - 1; i >= start; --i) {
            if (a[i] > a[i + 1]) {
                swap(a[i], a[i + 1]);
                swapped = true;
                visualize(i, start);
                SDL_Delay(5);
            }
        }

        // increase the starting point, because
        // the last stage would have moved the next
        // smallest number to its rightful spot.
        ++start;
    }
}
int getNextGap(int gap)
{
    // Shrink gap by Shrink factor
    gap = (gap * 10) / 13;

    if (gap < 1)
        return 1;
    return gap;
}

// Function to sort a[0..n-1] using Comb Sort
void combSort(int a[], int n)
{
    // Initialize gap
    int gap = n;

    // Initialize swapped as true to make sure that
    // loop runs
    bool swapped = true;

    // Keep running while gap is more than 1 and last
    // iteration caused a swap
    while (gap != 1 || swapped == true)
    {
        // Find next gap
        gap = getNextGap(gap);

        // Initialize swapped as false so that we can
        // check if swap happened or not
        swapped = false;

        // Compare all elements with current gap
        for (int i = 0; i < n - gap; i++)
        {
            if (a[i] > a[i + gap])
            {
                swap(a[i], a[i + gap]);
                swapped = true;
                visualize(i, gap);
                SDL_Delay(5);
            }
        }
    }
}
void gnomeSort(int arr[], int n)
{
    int index = 0;

    while (index < n) {
        if (index == 0)
            index++;
        if (arr[index] >= arr[index - 1])
            index++;
        else {
            swap(arr[index], arr[index - 1]);
            index--;
            visualize(index-1, index);
            SDL_Delay(5);
        }
    }
    return;
}
void oddEvenSort(int arr[], int n)
{
    bool isSorted = false; // Initially array is unsorted

    while (!isSorted) {
        isSorted = true;

        // Perform Bubble sort on odd indexed element
        for (int i = 1; i <= n - 2; i = i + 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                isSorted = false;
            }
        }

        // Perform Bubble sort on even indexed element
        for (int i = 0; i <= n - 2; i = i + 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                isSorted = false;
                visualize(i - 1, i);
                SDL_Delay(5);
            }
        }
    }

    return;
}


void loadArr()
{
    memcpy(arr, Barr, sizeof(int) * arrSize);
}

void randomizeAndSaveArray()
{
    unsigned int seed = (unsigned)time(NULL);
    srand(seed);
    for (int i = 0; i < arrSize; i++)
    {
        int random = rand() % (SCREEN_HEIGHT);
        Barr[i] = random;
    }
}

void execute()
{
    if (!init())
    {
        cout << "SDL Initialization Failed.\n";
    }
    else
    {
        randomizeAndSaveArray();
        loadArr();
        const auto start = std::chrono::high_resolution_clock::now();
        
        SDL_Event e;
        bool quit = false;
        while (!quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                    complete = false;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    switch (e.key.keysym.sym)
                    {
                    case(SDLK_q):
                        quit = true;
                        complete = false;
                        cout << "\nEXITING SORTING VISUALIZER.\n";
                        break;
                    case(SDLK_0):
                        randomizeAndSaveArray();
                        complete = false;
                        loadArr();
                        cout << "\nNEW RANDOM LIST GENERATED.\n";
                        break;
                    case(SDLK_1):
                        loadArr();
                        cout << "\nSELECTION SORT STARTED.\n";
                        complete = false;
                        selectionSort();
                        //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\nSELECTION SORT COMPLETE.\n";
                       // std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    case(SDLK_2):
                        loadArr();
                        cout << "\nINSERTION SORT STARTED.\n";
                        complete = false;
                        insertionSort();
                        //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\nINSERTION SORT COMPLETE.\n";
                       //std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    case(SDLK_3):
                        loadArr();
                        cout << "\nBUBBLE SORT STARTED.\n";
                        complete = false;
                        bubbleSort();
                       //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\nBUBBLE SORT COMPLETE.\n";
                        //std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    case(SDLK_4):
                        loadArr();
                        cout << "\nMERGE SORT STARTED.\n";
                        complete = false;
                        //mergeSort(arr, 0, arrSize - 1);
                        //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\nMERGE SORT COMPLETE.\n";
                       // std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    case(SDLK_5):
                        loadArr();
                        cout << "\nQUICK SORT STARTED.\n";
                        complete = false;
                        quickSort(arr, 0, arrSize - 1);
                        //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\nQUICK SORT COMPLETE.\n";
                       // std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    case(SDLK_6):
                        loadArr();
                        cout << "\nHEAP SORT STARTED.\n";
                        complete = false;
                        inplaceHeapSort(arr, arrSize);
                        //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\nHEAP SORT COMPLETE.\n";
                        //std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                   /* case(SDLK_7):
                        loadArr();
                        cout << "\nTIM SORT STARTED.\n";
                        complete = false;
                        timSort(arr, 130);
                        complete = true;
                        cout << "\nTIM SORT COMPLETE.\n";
                        break;
                        */
                    case(SDLK_7):
                        loadArr();
                        cout << "\nSHELL SORT STARTED.\n";
                        complete = false;
                        shellSort(arr,130);
                       // const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\nSHELL SORT COMPLETE.\n";
                      // std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    /*case(SDLK_9):
                        loadArr();
                        cout << "\nTREE SORT STARTED.\n";
                        complete = false;
                        treeSort(arr, 130);
                        complete = true;
                        cout << "\nTREE SORT COMPLETE.\n";
                        break;
                        */
                    case(SDLK_8):
                        loadArr();
                        cout << "\nCYCLE SORT STARTED.\n";
                        complete = false;
                        cycleSort(arr, 130);
                        //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\nCYCLE SORT COMPLETE.\n";
                       // std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    /*case(SDLK_b):
                        loadArr();
                        cout << "\nSTRAND SORT STARTED.\n";
                        complete = false;
                        strandSort(??);
                        complete = true;
                        cout << "\nSTRAND SORT COMPLETE.\n";
                        break;
                        */
                    case(SDLK_9):
                        loadArr();
                        cout << "\nSHAKER SORT STARTED.\n";
                        complete = false;
                        CocktailSort(arr, 130);
                        //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\nSHAKER SORT COMPLETE.\n";
                       // std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    case(SDLK_a):
                        loadArr();
                        cout << "\COMB SORT STARTED.\n";
                        complete = false;
                        combSort(arr, 130);
                        //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\COMB SORT COMPLETE.\n";
                       //std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    case(SDLK_b):
                        loadArr();
                        cout << "\GNMOE SORT STARTED.\n";
                        complete = false;
                        gnomeSort(arr, 130);
                       //const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\GNOME SORT COMPLETE.\n";
                        //std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;
                    case(SDLK_c):
                        loadArr();
                        cout << "\ODD/EVEN SORT STARTED.\n";
                        complete = false;
                        oddEvenSort(arr, 130);
                        const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                        complete = true;
                        cout << "\ODD/EVEN SORT COMPLETE.\n";
                        std::cout << "Elapsed Time:\t" << elapsed.count() << " s\n";
                        break;

                    }
                }
            }
            visualize();
        }
        close();
    }
}

bool controls()
{
    cout << "WARNING: Giving repetitive commands may cause latency and the visualizer may behave unexpectedly. Please give a new command only after the current command's execution is done.\n\n"
        << "Available Controls inside Sorting Visualizer:-\n"
        << "    Use 0 to Generate a different randomized list.\n"
        << "    Use 1 to start Selection Sort Algorithm.\n"
        << "    Use 2 to start Insertion Sort Algorithm.\n"
        << "    Use 3 to start Bubble Sort Algorithm.\n"
        << "    Use 4 to start Merge Sort Algorithm.\n"
        << "    Use 5 to start Quick Sort Algorithm.\n"
        << "    Use 6 to start Heap Sort Algorithm.\n"
        << "    Use 7 to start Shell Sort Algorithm.\n"
        << "    Use 8 to start Cycle Sort Algorithm.\n"
        << "    Use 9 to start Shaker Sort Algorithm.\n"
        << "    Use a to start Comb Sort Algorithm.\n"
        << "    Use b to start Gnome Sort Algorithm.\n"
        << "    Use c to start Odd/Even Sort Algorithm.\n"
        << "PRESS ENTER TO START SORTING VISUALIZER...\n\n"
        << "Or type -1 and press ENTER to quit the program.";

    string s;
    getline(cin, s);
    if (s == "-1")
    {
        return false;
    }
    //else if(s=="\n")
    //{
    //    return true;
    //}
    return true;
}

void intro()
{
    cout << "==============================Sorting Visualizer==============================\n\n"
        << "Visualization of different sorting algorithms in C++ with SDL2 Library. A sorting algorithm is an algorithm that puts the elements of a list in a certain order. While there are a large number of sorting algorithms, in practical implementations a few algorithms predominate.\n"
        << "In this implementation of sorting visualizer, we'll be looking at some of these sorting algorithms and visually comprehend their working.\n"
        << "The sorting algorithms covered here are Selection Sort, Insertion Sort, Bubble Sort, Merge Sort, Quick Sort and Heap Sort.\n"
        << "The list size is fixed to 130 elements. You can randomize the list and select any type of sorting algorithm to call on the list from the given options. Here, all sorting algorithms will sort the elements in ascending order. The sorting time being visualized for an algorithm is not exactly same as their actual time complexities. The relatively faster algorithms like Merge Sort, etc. have been delayed so that they could be properly visualized.\n\n"
        << "Press ENTER to show controls...";

    string s;
    getline(cin, s);
    if (s == "\n")
    {
        return;
    }
}

int main(int argc, char* args[])
{
    intro();

    while (1)
    {
        cout << '\n';
        if (controls())
            execute();
        else
        {
            cout << "\nEXITING PROGRAM.\n";
            break;
        }
    }

    return 0;
}
