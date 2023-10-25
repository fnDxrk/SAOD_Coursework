    printTreeKey(root->left, num);
    for (size_t i = 0; i < root->data.size(); i++) {
        if (root->data[i].head->data->numberApartment == num) {
            std::cout << root->data[i].head->data->fullname << "\t|| "
                << root->data[i].head->data->street << "\t|| "
                << root->data[i].head->data->numberHouse << "\t|| "
                << root->data[i].head->data->numberApartment << "\t|| "
                << root->data[i].head->data->dateSettle << "||" << std::endl;
        }
    }