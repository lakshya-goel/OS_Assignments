SYSCALL_DEFINE3(kernel_2D_memcpy, float **, src, float **, dest, int, len)
{
    float mat[4][4];
    if(__copy_from_user(mat, src, sizeof(mat)))
    {
      return -EFAULT;
    }
    if(__copy_to_user(dest, mat, sizeof(mat)))
    {
      return -EFAULT;
    }
    return 0;
}
