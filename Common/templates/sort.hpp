#pragma once
template<class T>void __quickSort_private__(T* a, long count)
{
// �� ����� - ������ a[], N - ������ �������.
  long i = 0, j = count; 		// ��������� ��������� �� �������� �����
  T temp, p;

  p = a[ count>>1 ];		// ����������� �������
  // ��������� ����������
  do {
    while ( p > a[i]) i++;
    while ( a[j] > p ) j--;

    if (i <= j) {
      temp = a[i]; a[i] = a[j]; a[j] = temp;
      ++i; --j;
    }
  } while ( i<=j );
  // ����������� ������, ���� ����, ��� �����������
  if ( j > 0 ) __quickSort_private__(a, j);
  if ( count > i ) __quickSort_private__(a+i, count-i);
}

template<class T>void QuickSort(T* a, long count){__quickSort_private__(a, count - 1);}

