#include <iostream>
#include <memory>
#include <chrono>
#include <cstdlib>
#include <array>
#include <algorithm>
#include <cmath>
#include <fstream>

bool sort_check(bool rising, int *data, int number_of_elements)
{
  bool output = 1;

  if (rising)
  {
    for (int i = 1; i < number_of_elements; i++)
    {
      if (output)
      {
        output = data[i] >= data[i - 1] ? 1 : 0;
      }
    }
  }
  else
  {
    for (int i = 1; i < number_of_elements; i++)
    {
      if (output)
      {
        output = data[i] <= data[i - 1] ? 1 : 0;
      }
    }
  }
  return output;
}

void quicksort(int left_element, int right_element, int *data, bool different_direction)
{
  if (different_direction)
  {
    int i, j, pivot;

    i = (left_element + right_element) / 2;
    pivot = data[i];
    data[i] = data[right_element];
    for (j = i = left_element; i < right_element; i++)
      if (data[i] < pivot)
      {
        std::swap(data[i], data[j]);
        j++;
      }
    data[right_element] = data[j];
    data[j] = pivot;
    if (left_element < j - 1)
      quicksort(left_element, j - 1, data, different_direction);
    if (j + 1 < right_element)
      quicksort(j + 1, right_element, data, different_direction);
  }
  else
  {
    int i, j, pivot;

    i = (left_element + right_element) / 2;
    pivot = data[i];
    data[i] = data[right_element];
    for (j = i = left_element; i < right_element; i++)
      if (data[i] > pivot)
      {
        std::swap(data[i], data[j]);
        j++;
      }
    data[right_element] = data[j];
    data[j] = pivot;
    if (left_element < j - 1)
      quicksort(left_element, j - 1, data, different_direction);
    if (j + 1 < right_element)
      quicksort(j + 1, right_element, data, different_direction);
  }
}

void merge_sort(int index_of_first_element, int index_of_last_element, int *data, int *merging_array)
{
  int index_of_middle_element, index_of_data_first_half, index_of_data_second_half, i; // i -> index in merging_array array

  index_of_middle_element = (index_of_first_element + index_of_last_element + 1) / 2;
  if (index_of_middle_element - index_of_first_element > 1)
    merge_sort(index_of_first_element, index_of_middle_element - 1, data, merging_array);
  if (index_of_last_element - index_of_middle_element > 0)
    merge_sort(index_of_middle_element, index_of_last_element, data, merging_array);
  index_of_data_first_half = index_of_first_element;
  index_of_data_second_half = index_of_middle_element;
  for (i = index_of_first_element; i <= index_of_last_element; i++)
    merging_array[i] = ((index_of_data_first_half == index_of_middle_element) || ((index_of_data_second_half <= index_of_last_element) &&
     (data[index_of_data_first_half] > data[index_of_data_second_half]))) ? data[index_of_data_second_half++] : data[index_of_data_first_half++];
  for (i = index_of_first_element; i <= index_of_last_element; i++)
    data[i] = merging_array[i];
}

void make_heap(const int number_of_elements, const int root, int *data)
{
  int new_root = root;
  int left = 2 * root + 1;
  int right = 2 * root + 2;

  if (left < number_of_elements && data[left] > data[new_root])
  {
    new_root = left;
  }

  if (right < number_of_elements && data[right] > data[new_root])
  {
    new_root = right;
  }

  if (new_root != root)
  {
    std::swap(data[root], data[new_root]);
    make_heap(number_of_elements, new_root, data);
  }
}

void heap_sort(int const number_of_elements, int *data)
{
  for (int i = number_of_elements / 2 - 1; i >= 0; --i)
  {
    make_heap(number_of_elements, i, data);
  }

  for (int i = number_of_elements - 1; i > 0; --i)
  {
    std::swap(data[0], data[i]);
    make_heap(i, 0, data);
  }
}

void sort_get_time_check(int number_of_elements)
{
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop_time;
  double execution_time = 0;
  const float percentage[8] = {0, 25, 50, 75, 95, 99, 99.7, 100};
  std::ofstream results("sort_results.txt", std::ofstream::app);
  std::cout.precision(10);
  for (size_t k = 0; k < 8; k++)
  {
    if (k != 7)
    {
      results << "ARRAY SIZE: " << number_of_elements << " "
              << "array sorted in " << percentage[k] << "%" << std::endl;
    }
    else
    {
      results << "ARRAY SIZE: " << number_of_elements << " "
              << "array sorted backwards" << std::endl;
    }
    for (size_t i = 0; i < 100; i++)
    {
      int *data = new int[number_of_elements];
      int *data_adress = &data[0];
      for (int i = 0; i < number_of_elements; i++)
      {
        data[i] = rand() % 1000;
      }
      // if (!sort_check(1, data, number_of_elements))
      // {
      //     results << "nieposegregowane przed" << std::endl;
      // }

      int end = (percentage[k] * (number_of_elements - 1)) / 100;
      quicksort(0, end, data_adress, 0);
      if (k == 7)
      {
        quicksort(0, end, data_adress, 0);
        if (!sort_check(0, data_adress, number_of_elements))
        {
          results << "nie odwrocone przed" << std::endl;
        }
      }
      else
      {
        quicksort(0, end, data_adress, 1);
      }

      start_time = std::chrono::high_resolution_clock::now();
      quicksort(0, (number_of_elements - 1), data_adress, 1);
      stop_time = std::chrono::high_resolution_clock::now();
      execution_time += std::chrono::duration<double, std::milli>(stop_time - start_time).count();
      if (!sort_check(1, data_adress, number_of_elements))
      {
        results << "Sortowanie nieudane" << std::endl;
      }
      delete[] data;
    }
    results << "Execution time: " << std::fixed << execution_time / 100 << " QUICKSORT" << std::endl;
    execution_time = 0;

    for (size_t i = 0; i < 100; i++)
    {
      int *data = new int[number_of_elements];
      int *data_adress = &data[0];
      int *merging_array = new int[number_of_elements];
      int *merging_array_address = &merging_array[0];
      for (int i = 0; i < number_of_elements; i++)
      {
        data[i] = rand() % 1000;
      }
      int end = (percentage[k] * (number_of_elements - 1)) / 100;
      if (k == 7)
      {
        quicksort(0, end, data_adress, 0);
        if (!sort_check(0, data_adress, number_of_elements))
        {
          results << "nie odwrocone przed" << std::endl;
        }
      }
      else
      {
        quicksort(0, end, data_adress, 1);
      }

      start_time = std::chrono::high_resolution_clock::now();
      merge_sort(0, (number_of_elements - 1), data_adress, merging_array_address);
      stop_time = std::chrono::high_resolution_clock::now();
      execution_time += std::chrono::duration<double, std::milli>(stop_time - start_time).count();
      if (!sort_check(1, data_adress, number_of_elements))
      {
        results << "Sortowanie nieudane" << std::endl;
      }
      delete[] data;
      delete[] merging_array;
    }
    results << "Execution time: " << std::fixed << execution_time / 100 << " MERGE SORT" << std::endl;
    execution_time = 0;
    for (size_t i = 0; i < 100; i++)
    {
      int *data = new int[number_of_elements];
      int *data_adress = &data[0];
      for (int i = 0; i < number_of_elements; i++)
      {
        data[i] = rand() % 1000;
      }
      int end = (percentage[k] * (number_of_elements - 1)) / 100;

      if (k == 7)
      {
        quicksort(0, end, data_adress, 0);
        if (!sort_check(0, data_adress, number_of_elements))
        {
          results << "nie odwrocone przed" << std::endl;
        }
      }
      else
      {
        quicksort(0, end, data_adress, 1);
      }

      start_time = std::chrono::high_resolution_clock::now();
      heap_sort(number_of_elements, data_adress);
      stop_time = std::chrono::high_resolution_clock::now();
      execution_time += std::chrono::duration<double, std::milli>(stop_time - start_time).count();

      if (!sort_check(1, data_adress, number_of_elements))
      {
        results << "Sortowanie nieudane" << std::endl;
      }
      delete[] data;
    }
    results << "Execution time: " << std::fixed << execution_time / 100 << " HEAP SORT" << std::endl;
    execution_time = 0;
    if (k == 7)
    {
      results << std::endl
              << std::endl;
    }
  }
  results.close();
}
