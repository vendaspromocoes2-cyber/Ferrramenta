import {
  downloadTextFile,
  downloadJsonFile,
  downloadCsvFile,
  downloadFileFromBlob,
} from './fileDownload';

describe('fileDownload utilities', () => {
  let createElementSpy: jest.SpyInstance;
  let mockLink: any;

  beforeEach(() => {
    mockLink = {
      href: '',
      download: '',
      click: jest.fn(),
    };

    createElementSpy = jest.spyOn(document, 'createElement');
    createElementSpy.mockReturnValue(mockLink);

    jest.spyOn(document.body, 'appendChild').mockImplementation(() => mockLink);
    jest.spyOn(document.body, 'removeChild').mockImplementation(() => mockLink);
    
    global.URL.createObjectURL = jest.fn(() => 'blob:mock-url');
    global.URL.revokeObjectURL = jest.fn();
  });

  afterEach(() => {
    jest.restoreAllMocks();
  });

  describe('downloadTextFile', () => {
    it('should create a text file download with correct content', () => {
      const content = 'Hello, World!';
      const filename = 'test.txt';

      downloadTextFile(content, filename);

      expect(mockLink.click).toHaveBeenCalled();
      expect(mockLink.download).toBe(filename);
      expect(global.URL.createObjectURL).toHaveBeenCalled();
      expect(global.URL.revokeObjectURL).toHaveBeenCalled();
    });

    it('should use custom mime type when provided', () => {
      const content = 'Custom content';
      const filename = 'custom.txt';
      const mimeType = 'text/custom';

      downloadTextFile(content, filename, mimeType);

      expect(mockLink.click).toHaveBeenCalled();
      expect(mockLink.download).toBe(filename);
    });
  });

  describe('downloadJsonFile', () => {
    it('should create a JSON file download with correct content', () => {
      const data = { name: 'Test', value: 123 };
      const filename = 'test.json';

      downloadJsonFile(data, filename);

      expect(mockLink.click).toHaveBeenCalled();
      expect(mockLink.download).toBe(filename);
    });
  });

  describe('downloadCsvFile', () => {
    it('should create a CSV file download with correct content', () => {
      const rows = [
        ['Name', 'Age', 'City'],
        ['John', '30', 'NYC'],
        ['Jane', '25', 'LA'],
      ];
      const filename = 'test.csv';

      downloadCsvFile(rows, filename);

      expect(mockLink.click).toHaveBeenCalled();
      expect(mockLink.download).toBe(filename);
    });
  });

  describe('downloadFileFromBlob', () => {
    it('should create a download from blob data', () => {
      const blob = new Blob(['test content'], { type: 'text/plain' });
      const filename = 'test.txt';

      downloadFileFromBlob(blob, filename);

      expect(mockLink.click).toHaveBeenCalled();
      expect(mockLink.download).toBe(filename);
      expect(global.URL.createObjectURL).toHaveBeenCalledWith(blob);
      expect(global.URL.revokeObjectURL).toHaveBeenCalled();
    });
  });
});
